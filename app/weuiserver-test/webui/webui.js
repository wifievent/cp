let url = 'http://wifievent.io/api';
url = 'http://localhost:3001';

window.onload = async function () {
  const sessRes = await axios.get(`${url}/user/session`, {
    withCredentials: true
  });
  if (sessRes.status !== 200)
    return;

  exchangeUI();

  const pageRes = await axios.get(`${url}/cp/page`, {
    withCredentials: true,
  }).catch(err => {
    console.log(err.response);
  });
  if (pageRes.status !== 200)
    return;
  
  $('#browsers').empty();
  for (const page of pageRes.data) {
    const option = $(`<option value='${page.id}'>${page.name}</option>`);
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

  const loginRes = await axios.post(`${url}/user/login`, body, {
    withCredentials: true,
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

function cpStatus() {
  if($('#button').text() === 'Start') {
    //start logic
    $('#buttonbox').attr('class','red-button-wrap button-wrap');
    $('#button').text('Stop');
  }
  else if($('#button').text() === 'Stop') {
    //stop logic
    $('#buttonbox').attr('class','blue-button-wrap button-wrap');
    $('#button').text('Start');
  }
}