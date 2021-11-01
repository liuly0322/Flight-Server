const url = "http://127.0.0.1:1316/"
function toggleHidden(s) {
  document.querySelector(s).classList.toggle('hidden')
}

// 下面是登录逻辑的判断

function myTick() {
  const out = document.querySelector('#myTick')
  const username = sessionStorage.getItem('username')
  const password = sessionStorage.getItem('password')
  axios.get(`${url}?username=${username}&password=${password}&state=3`)
    .then((response) => {
      if (response.data) {  // 如果有订票记录
        let res = '<table class="table" border="1"><thead><tr><th>航班号</th><th>状态</th><th>舱位</th><th>票数</th><th>退票</th></tr></thead>'
        response.data.split('\n').forEach((content) => {
          if (content !== '') {
            const items = content.split(' ');
            // items 分别是航班号，是否订上，id，几等仓，多少张
            res +=
              `<tr>
                  <td>${items[0]}</td>
                  <td>${items[1] == '1' ? '完成' : '候补'}</td>
                  <td>${parseInt(items[3]) + 1}</td>
                  <td>${items[4]}</td>
                  <td>${items[1] == '1' ? `<button onclick="refund(${items[2]});">退票</button>` : '候补'}</td>
              </tr>`
          }
        })
        res += '</table>'
        out.innerHTML = res
      } else {
        out.innerHTML = "未查询到订票记录"
      }
    }).catch((error) => {
      console.log(error)
    })
}
if (sessionStorage.getItem('username') && sessionStorage.getItem('password')) {
  // 显示注销和我订的票
  toggleHidden('#quit')
  toggleHidden('#mine')
  // 这里还需要渲染我订的票
  myTick();
} else {
  // 显示注册和登录
  toggleHidden('#regi')
  toggleHidden('#login')
}

function regi() {   // 注册
  const username = document.querySelector('#regiName').value
  const password = document.querySelector('#regiPass').value
  axios.get(`${url}?username=${username}&password=${password}&state=1`)
    .then((response) => {
      alert(response.data)
      if (response.data == '注册成功') {
        sessionStorage.setItem('username', username)
        sessionStorage.setItem('password', password)
      }
      location.reload()
    }).catch((error) => {
      console.log(error)
    })
}

function login() {  // 登录
  const username = document.querySelector('#loginName').value
  const password = document.querySelector('#loginPass').value
  axios.get(`${url}?username=${username}&password=${password}&state=2`)
    .then((response) => {
      alert(response.data)
      if (response.data == "登录成功") {
        sessionStorage.setItem('username', username)
        sessionStorage.setItem('password', password)
      }
      location.reload()
    }).catch((error) => {
      console.log(error)
    })
}

function book(flight) {
  if (!sessionStorage.getItem('username') || !sessionStorage.getItem('password')) {
    alert('请先登录')
    return
  }
  toggleHidden('.book')
  sessionStorage.setItem('flight', flight)
}

function refund(id) {
  const username = sessionStorage.getItem('username');
  const password = sessionStorage.getItem('password');
  axios.get(`${url}?username=${username}&password=${password}&state=6&id=${id}`)
    .then(() => { query(); myTick() })
    .catch((error) => console.log(error))
}

function bookTicket() {
  const username = sessionStorage.getItem('username');
  const password = sessionStorage.getItem('password');
  const state = document.querySelector('#wait').checked ? 5 : 4
  const flight = sessionStorage.getItem('flight')
  const grade = document.querySelector('#bookGrade').value - 1
  const num = document.querySelector('#bookNum').value

  // 接下来发送订票申请

  axios.get(`${url}?username=${username}&password=${password}&state=${state}&flight=${flight}&grade=${grade}&num=${num}`)
    .then((response) => {
      if (response.data == '订票失败') {
        query(grade, num)
      } else {
        query();
      }
      myTick();
      toggleHidden('.book')
    })
    .catch((error) => console.log(error))
}

function query(grade, num) {
  const out = document.querySelector('#queryRes');   // 结果在此展示
  let s = document.querySelector('#s').value
  const reg = new RegExp("[\\u4E00-\\u9FFF]+", "g"); // 判断汉字
  if (reg.test(s)) {
    s = Pinyin.convertToPinyin(s).toLowerCase()
  } else {
    s = s.toUpperCase();
  }
  s = `${url}?s=${s}`;
  if (grade != undefined && num != undefined) {
    // 如果有这两个参数，说明查询的时候还需要附加
    s += `&grade=${grade}&num=${num}`
  }
  axios.get(s)
    .then((response) => {
      if (response.data) {
        // 此时需要对每一个查询结果渲染
        let res = ''
        if (grade != undefined && num != undefined) {
          // 如果有这两个参数，附加一个标题
          res += '<h2>给您推荐满足要求的同目的地航线：</h2>'
        }
        res += '<table class="table" border="1"><thead><tr><th>目的地</th><th>航班号</th><th>飞机号</th><th>周</th><th>一等舱</th><th>二等舱</th><th>三等舱</th><th>订票</th></tr></thead>'
        response.data.split('\n').forEach((content) => {
          if (content !== '') {
            const items = content.split(' ');
            // items 分别是对应航班的目的地，航班号，飞机号，周几，
            // 三个舱的最大定额和剩余票数
            res +=
              `<tr>
              <th>${items[0]}</th>
              <th>${items[1]}</th>
              <th>${items[2]}</th>
              <th>${items[3]}</th>
              <th>${items[7]}/${items[4]}</th>
              <th>${items[8]}/${items[5]}</th>
              <th>${items[9]}/${items[6]}</th>
              <th><button onclick="book('${items[1]}');">订票</button></th>
              </tr>`
          }
        })
        res += '</table>'
        out.innerHTML = res
      } else {
        // 此时需要渲染：没有查询结果
        out.innerHTML = 'oops, not find'
      }
    })
    .catch((error) => {
      console.log(error)
    })
}

function quit() {
  sessionStorage.clear();
  location.reload();
}