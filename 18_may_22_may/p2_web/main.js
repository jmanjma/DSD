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

updateGraph(function(string1, string2, string3, value1, value2, value3){
const ctx = document.getElementById('chart').getContext('2d');
	label1 = string1;
	label2 = string2;
	label3 = string3;
	value1 = num1;
	value2 = num2;
	value3 = num3;

const myChart = new Chart(ctx, {
    type: 'bar',
    data: {
        //labels: xlabels,
        labels: [label1, label2, label3],
        datasets: [{
            label: '# of Votes',
            data: [num1,num2,num3],
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

}
	    
});
