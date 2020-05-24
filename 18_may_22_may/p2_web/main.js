$(document).ready(function() {
  // Start 1-second timer to call RESTful endpoint
  setInterval(function() {
    $.ajax({
      url: '/get_cpu_usage',
      dataType: 'json',
     // data: {Server1: $('server1_new').text(), Server2: $('server2_new').text(), Server3: $('#server3_new').text()},
     // data: {votos1: $('#votos1_new').val(), votos2: $('#votos2_new').val(), votos3: $('#votos3_new').val()},
    //data: {"Server 1": serv1, "Server 2": serv2, "Server 3": serv3, votos1: 3, votos2: vot2, votos3: vot3 },
      success: function(json) {
        $('#cpu_usage').text(json.result + '% ');
      }
    });
  }, 1000);

	//xlabels.push(servers);

  // Initialize graph

const ctx = document.getElementById('chart').getContext('2d');
//const xlabels = [];
const myChart = new Chart(ctx, {
    type: 'bar',
    data: {
        //labels: xlabels,
        labels: ['Server 1', 'Server 2', 'Server 3'],
        datasets: [{
            label: '# of Votes',
            data: [12,19, 13],
            backgroundColor: [
                'rgba(255, 99, 132, 0.2)',
                'rgba(54, 162, 235, 0.2)',
                'rgba(255, 206, 86, 0.2)' 
            ],
            borderColor: [
                'rgba(255, 99, 132, 1)',
                'rgba(54, 162, 235, 1)',
                'rgba(255, 206, 86, 1)'
            ],
            borderWidth: 1
        }]
    },
    options: {
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true
                }
            }]
        }
    }
});



    
});
