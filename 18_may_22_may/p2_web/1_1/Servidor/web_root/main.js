$(document).ready(function() {
  // Start 1-second timer to call RESTful endpoint
  setInterval(function() {
    $.ajax({
      url: '/get_cpu_usage',
      dataType: 'json',
      success: function(json) {
        $('#cpu_usage').text(JSON.stringify(json));
        var parent = document.getElementById('graphs');
        parent.innerHTML += "<h2>" + JSON.stringify(json) + "</h2><br>";
      }
    });
  }, 1000);

  // Create Websocket connection. For simplicity, no reconnect logic is here.
  // var ws = new WebSocket('ws://' + location.host);
  // ws.onmessage = function(ev) {
  //   var parent = document.getElementById('graphs');
  //   parent.innerHTML += "<h3>" + ev.data + "</h3><br>";
  //   // updateGraph(ev.data);
  // };
});
