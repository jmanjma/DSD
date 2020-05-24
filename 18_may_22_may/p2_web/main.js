$(document).ready(function() {
  // Start 1-second timer to call RESTful endpoint
  setInterval(function() {
    $.ajax({
      url: '/get_cpu_usage',
      dataType: 'json',
 success: function(json) {
        $('#cpu_usage').text(json.result + '% ')	
      }
    });
  }, 1000);
		
  // Initialize graph

updateGraph('hola', 'hola2', 'hola3', 17, 18, 19);

function updateGraph(string1, string2, string3, value1, value2, value3){
const ctx = document.getElementById('chart').getContext('2d');
	var label1 = string1;
	var label2 = string2;
	var label3 = string3;
	var num1 = value1;
	var num2 = value2;
	var num3 = value3;
const myChart = new Chart(ctx, {
    type: 'bar',
    data: {
        labels: [label1, label2, label3],
        datasets: [{
            label: '# of Votes',
            data: [num1, num2, num3],
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
	return myChart;

}
	    
});
