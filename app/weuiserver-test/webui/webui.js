let url = 'http://192.168.1.10:3001'

function buttonClick() {
  const uid = document.getElementById('uid').value;
  const pw = document.getElementById('pw').value;

  const body = { uid, pw };

  let pageList = [];

  axios.post(url+'/user/login', body, {
    withCredentials: true
  })
  .then(function (response) {
    if (response.status === 200) {
      exchangeUI();

      axios.get(url+"/cp/page")
      .then(function (response) {
        if (response.status === 200) {
          console.log(response.data);
          console.log(typeof(response.data));
        }
        //pageList.push(response.data);
      })
    }
    else if(response.status === 401) {
      window.alert("Login Fail");
    }
    else{
      window.alert("error");
    }
  });
}

function exchangeUI(pageList) {
  $("#idbox").remove();
  $("#pwbox").remove();
  $("#buttonbox").remove();
  $("#main").append(`<div class="input-wrap" id="idbox"> \
                      <select class="input-wrap" id="browsers" name="browsers">\
                        <option value=""></option>\
                        <option value=""></option>\
                        <option value=""></option>\
                        <option value=""></option>\
                        <option value=""></option>\
                        <option value=""></option>\
                      </select>\
                    </div> \
                    <div class="blue-button-wrap button-wrap" id="buttonbox"> \
                      <button type="button" onclick="cpStatus()" id="button">Start</button> \
                    </div>`);
}

function cpStatus() {
  if($("#button").text() === "Start") {
    //start logic
    $("#buttonbox").attr('class','red-button-wrap button-wrap');
    $("#button").text("Stop");
  }
  else if($("#button").text() === "Stop") {
    //stop logic
    $("#buttonbox").attr('class','blue-button-wrap button-wrap');
    $("#button").text("Start");
  }
}