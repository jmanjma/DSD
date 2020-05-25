$(document).ready(function() {
  // Start 1-second timer to call RESTful endpoint
  setInterval(function() {
    $.ajax({
      url: '/get_cpu_usage',
      dataType: 'json',
        success: function(json) {
            var parent = document.getElementById('graphs');
            var cadena = "Votos procesados: " + Object.values(json).reduce((a, b) => a + b, 0) + "<br>" ;
            Object.keys(json).forEach(key => {
                cadena += key + ": " + json[key] + "<br>";
            });
            // $('#cpu_usage').text(cadena);
            parent.innerHTML = cadena;
            // $('#cpu_usage').text(JSON.stringify(json));
                updateGraph(json);
            }
    });
  }, 1000);

function updateGraph(json){
    const ctx = document.getElementById('chart').getContext('2d');
    const myChart = new Chart(ctx, {
        type: 'bar',
        data: {
            labels: Object.keys(json),
            datasets: [{
                label: '# of Votes',
                data: Object.values(json),
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
            animation: {
                duration: 0
            },
            scales: {
                yAxes: [{
                    ticks: {
                        beginAtZero: true
                    }
                }]
            }
        }
    });
    return myChart;

}
        
});
