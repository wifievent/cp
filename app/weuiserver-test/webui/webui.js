let url = 'http://wifievent.io/api';
url = 'http://localhost:3001';

const webReq = axios.create({
  baseURL: url,
  withCredentials: true,
  timeout: 1000,
})

window.onload = async function () {
  const sessRes = await webReq({
    url: '/user/session',
  });
  if (sessRes.status !== 200)
    return;

  exchangeUI();

  const pageRes = await webReq({
    url: '/cp/page',
  })
  if (pageRes.status !== 200)
    return;
  
  $('#browsers').empty();
  for (const page of pageRes.data) {
    const option = $(`<option value='${page.pid}'>${page.name}</option>`);
    $('#browsers').append(option);
  }
};

document.querySelector('#button').addEventListener('click', async function () {
  const uid = document.querySelector('#uid').value;
  const pw = document.querySelector('#pw').value;
  if (!uid || !pw) {
    alert('fill in the blank');
    return;
  }

  const body = { uid, pw };

  const loginRes = await webReq({
    method: 'POST',
    url: '/user/login',
    data: body,
  }).catch(err => {
    alert(err.response.statusText);
    return;
  });
  if (loginRes.status !== 200)
    return;

  alert('login success');
  location.reload();
});

function exchangeUI(pageList) {
  $('#idbox').remove();
  $('#pwbox').remove();
  $('#buttonbox').remove();
  $('#main').append(`<div class='input-wrap' id='idbox'> \
                      <select class='input-wrap' id='browsers' name='browsers'>\
                      </select>\
                    </div> \
                    <div class='blue-button-wrap button-wrap' id='buttonbox'> \
                      <button type='button' onclick='cpStatus()' id='button'>Start</button> \
                    </div>`);
}

async function cpReq(status, pid) {
  await axios.post(`/${status}`, {
    url: `${url}/cp/pagep/render/${pid}`,
  }).catch(err => {
    return 0;
  });
  return 1;
}

async function cpStatus() {
  const status = $('button').text();
  const pid = document.querySelector('#browsers').value;
  const res = await cpReq(status.toLowerCase(), pid);
  if(!res) {
    alert(`${status} Failed`);
    return;
  }

  const buttonColor = status == 'Start' ? 'red' : 'blue';
  $('#buttonbox').attr('class',`${buttonColor}-button-wrap button-wrap`);
  $('#button').text(status);
}