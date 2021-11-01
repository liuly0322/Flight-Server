/*
 * @Author       : mark
 * @Date         : 2020-06-15
 * @copyleft Apache 2.0
 */
#include "httpconn.h"
using namespace std;

const char* HttpConn::srcDir;
std::atomic<int> HttpConn::userCount;
bool HttpConn::isET;

std::mutex HttpConn::mtx;
Ticket HttpConn::ticket;

HttpConn::HttpConn() {
    fd_ = -1;
    addr_ = {0};
    isClose_ = true;
};

HttpConn::~HttpConn() {
    Close();
};

void HttpConn::init(int fd, const sockaddr_in& addr) {
    assert(fd > 0);
    userCount++;
    addr_ = addr;
    fd_ = fd;
    writeBuff_.RetrieveAll();
    readBuff_.RetrieveAll();
    isClose_ = false;
    LOG_INFO("Client[%d](%s:%d) in, userCount:%d", fd_, GetIP(), GetPort(),
             (int)userCount);
}

void HttpConn::Close() {
    response_.UnmapFile();
    if (isClose_ == false) {
        isClose_ = true;
        userCount--;
        close(fd_);
        LOG_INFO("Client[%d](%s:%d) quit, UserCount:%d", fd_, GetIP(),
                 GetPort(), (int)userCount);
    }
}

int HttpConn::GetFd() const {
    return fd_;
};

struct sockaddr_in HttpConn::GetAddr() const {
    return addr_;
}

const char* HttpConn::GetIP() const {
    return inet_ntoa(addr_.sin_addr);
}

int HttpConn::GetPort() const {
    return addr_.sin_port;
}

ssize_t HttpConn::read(int* saveErrno) {
    ssize_t len = -1;
    do {
        len = readBuff_.ReadFd(fd_, saveErrno);
        if (len <= 0) {
            break;
        }
    } while (isET);
    return len;
}

ssize_t HttpConn::write(int* saveErrno) {
    ssize_t len = -1;
    do {
        len = writev(fd_, iov_, iovCnt_);
        if (len <= 0) {
            *saveErrno = errno;
            break;
        }
        if (iov_[0].iov_len + iov_[1].iov_len == 0) {
            break;
        } /* 传输结束 */
        else if (static_cast<size_t>(len) > iov_[0].iov_len) {
            iov_[1].iov_base =
                (uint8_t*)iov_[1].iov_base + (len - iov_[0].iov_len);
            iov_[1].iov_len -= (len - iov_[0].iov_len);
            if (iov_[0].iov_len) {
                writeBuff_.RetrieveAll();
                iov_[0].iov_len = 0;
            }
        } else {
            iov_[0].iov_base = (uint8_t*)iov_[0].iov_base + len;
            iov_[0].iov_len -= len;
            writeBuff_.Retrieve(len);
        }
    } while (isET || ToWriteBytes() > 10240);
    return len;
}

std::string HttpConn::processRequest() {
    // 对于 admin 登录
    if (request_.GetPost("admin") == "password") {
        std::string s = request_.GetPost("s");
        if (s.empty()) {  // 如果没有传入查询参数
            ticket.Save();
            return "文件已保存";
        } else {
            ticket.AdminQuery(s);
        }
    }

    // 对于查询操作
    std::string s = request_.Get("s");
    if (!s.empty()) {
        // 这个时候可能有 grade 和 num 传入
        std::string grade = request_.Get("grade");
        std::string num = request_.Get("num");
        if (grade.empty() || num.empty())
            return ticket.query(s);
        // 否则需要将 grade 和 num转换成整数
        int grade_ = atoi(grade.c_str());
        int num_ = atoi(num.c_str());
        if (grade_ < 0 || grade_ > 2 || num_ < 1)
            return "";
        return ticket.query(s, grade_, num_);
    }

    int state = atoi(request_.Get("state").c_str());  // 操作
    std::string username = request_.Get("username");  // 账户名
    std::string password = request_.Get("password");  // 密码
    std::string flight = request_.Get("flight");      // 航班号
    int grade = atoi(request_.Get("grade").c_str());  // 几等仓
    int num = atoi(request_.Get("num").c_str());      // 订票数量
    int id = atoi(request_.Get("id").c_str());        // 退票id

    if (state == 1) {
        if (ticket.Regi(username, password)) {
            return "注册成功";
        } else {
            return "注册失败，可能是用户名相同";
        }
    }

    if (!ticket.Login(username, password)) {
        return "登录失败";
    }

    if (state == 2)
        return "登录成功";

    if (state == 3)
        return ticket.MyTick(username);

    if (state == 4) {
        if (grade < 0 || grade > 2 || num < 1)
            return "";
        if (!ticket.Book(username, flight, grade, num, false)) {
            // 没定上票，需要推荐
            return "订票失败";
        }
    } else if (state == 5) {
        if (grade < 0 || grade > 2 || num < 1)
            return "";
        ticket.Book(username, flight, grade, num, true);
    } else if (state == 6) {
        ticket.Refund(username, id);
    }

    return "";
}

bool HttpConn::process() {
    request_.Init();
    if (readBuff_.ReadableBytes() <= 0) {
        return false;
    } else if (request_.parse(readBuff_)) {
        LOG_DEBUG("%s", request_.path().c_str());
        if (request_.method() == "POST" ||
            (request_.method() == "GET" && !request_.GetEmpty())) {
            mtx.lock();
            std::string res = processRequest();
            mtx.unlock();
            response_.Init(srcDir, request_.path(), request_.IsKeepAlive(),
                           200);
            response_.MakeResponse(writeBuff_, res);
        } else {
            response_.Init(srcDir, request_.path(), request_.IsKeepAlive(),
                           200);
            response_.MakeResponse(writeBuff_);
        }
    } else {
        response_.Init(srcDir, request_.path(), false, 400);
        response_.MakeResponse(writeBuff_);
    }

    /* 响应头 */
    iov_[0].iov_base = const_cast<char*>(writeBuff_.Peek());
    iov_[0].iov_len = writeBuff_.ReadableBytes();
    iovCnt_ = 1;

    /* 文件 */
    if (response_.FileLen() > 0 && response_.File()) {
        iov_[1].iov_base = response_.File();
        iov_[1].iov_len = response_.FileLen();
        iovCnt_ = 2;
    }
    LOG_DEBUG("filesize:%d, %d  to %d", response_.FileLen(), iovCnt_,
              ToWriteBytes());
    return true;
}
