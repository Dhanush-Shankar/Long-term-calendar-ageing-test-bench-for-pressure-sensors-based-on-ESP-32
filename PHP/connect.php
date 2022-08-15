
<?php 

   $host        = "host = ";
   $port        = "port = ";
   $dbname      = "dbname = ";
   $credentials = "user = '' password='';
   

   $db = pg_connect( "$host $port $dbname $credentials"  );
   $query = 'SELECT * FROM sensordatatest1 ORDER BY timestamp';
   $result = pg_query($query) or die('Error message: ' . pg_last_error());
   $data = array();
   $timestamp = array();
   $LoadCell = array();
   $sensor1 = array();
   $sensor2 = array();
   $sensor3 = array();
   $sensor4 = array();
   $sensor5 = array();
   $sensor6 = array();
   $sensor7 = array();
     $sensor8 = array();
   $sensor9 = array();
   $sensor10 = array();
     $sensor11 = array();
   $sensor12 = array();
   $sensor13 = array();
     $sensor14 = array();
   $sensor15 = array();

   while ($row = pg_fetch_row($result)) {
   $data[] = $row;
   }
   
   $i = sizeof($data);
   for ($x = 0; $x < $i; $x++) {
      array_push($timestamp,$data[$x][1]);
      array_push($LoadCell,(int)$data[$x][2]);
      array_push($sensor1,(int)$data[$x][3]);
      array_push($sensor2,(int)$data[$x][4]);
      array_push($sensor3,(int)$data[$x][5]);
      array_push($sensor4,(int)$data[$x][5]);
      array_push($sensor5,(int)$data[$x][6]);
      array_push($sensor6,(int)$data[$x][7]);
      array_push($sensor7,(int)$data[$x][8]);
      array_push($sensor8,(int)$data[$x][9]);
      array_push($sensor9,(int)$data[$x][10]);
      array_push($sensor10,(int)$data[$x][11]);
      array_push($sensor11,(int)$data[$x][12]);
      array_push($sensor12,(int)$data[$x][13]);
      array_push($sensor13,(int)$data[$x][14]);
      array_push($sensor14,(int)$data[$x][15]);
      array_push($sensor15,(int)$data[$x][16]);
    }

?>


<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content='10'; URL='http://localhost:8012/project7/connect.php'>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
  
<script	src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.8.0/Chart.bundle.min.js"></script>
<script src="https://code.jquery.com/jquery-3.3.1.slim.min.js" integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/popper.js@1.14.6/dist/umd/popper.min.js" integrity="sha384-wHAiFfRlMFy6i5SRaxvfOCifBUQy1xHdJ/yoi7FRNXMRBu5WHdZYu1hA6ZOblgut" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@4.2.1/dist/js/bootstrap.min.js" integrity="sha384-B0UglyR+jN6CkvvICOB2joaf5I4l3gm9GU6Hc1og6Ls7i6U/mkkaduKaBhlAXv9k" crossorigin="anonymous"></script>
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.2.1/dist/css/bootstrap.min.css" integrity="sha384-GJzZqFGwb1QTTN6wy59ffF1BuGJpLSa9DkKMp0DgiMDm4iYMj70gZWKYbI706tWS" crossorigin="anonymous">
<link rel="stylesheet" href="styles.css">  
<title>Project lab Embedded Systems</title>
</head>
<body>

<!-- Image and text -->
<nav class="navbar navbar-light bg-light">
  <a class="navbar-brand" href="#">
    
    Project lab Embedded Systems
  </a>
</nav>


</div>
<div  class="float-container">
    <div class="float-child">
      <canvas id="myChart"></canvas>
    </div>
    <div class="float-child">
      <canvas id="myChart_LoadCell"></canvas>
    </div>
    
</div>

  </div>
 


<script>
  // === include 'setup' then 'config' above ===

const labels = <?php echo json_encode($timestamp) ?>;

  const dataLoadCell = {
    labels: labels,
    datasets: [{
      label: 'LoadCell',
         data: <?php echo json_encode($LoadCell) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    }]
  };
  
  const data = {
     labels: labels,
     datasets: [{
      label: 'Sensor1',
         data: <?php echo json_encode($sensor1) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor2',
         data: <?php echo json_encode($sensor2) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor3',
         data: <?php echo json_encode($sensor3) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor4',
         data: <?php echo json_encode($sensor4) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor5',
         data: <?php echo json_encode($sensor5) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor6',
         data: <?php echo json_encode($sensor6) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
        {
      label: 'Sensor7',
         data: <?php echo json_encode($sensor7) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor8',
         data: <?php echo json_encode($sensor8) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor9',
         data: <?php echo json_encode($sensor9) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
        {
      label: 'Sensor10',
         data: <?php echo json_encode($sensor10) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor11',
         data: <?php echo json_encode($sensor11) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor12',
         data: <?php echo json_encode($sensor12) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
        {
      label: 'Sensor13',
         data: <?php echo json_encode($sensor13) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor14',
         data: <?php echo json_encode($sensor14) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
    {
      label: 'Sensor15',
         data: <?php echo json_encode($sensor15) ?>,
         fill: false,
         borderColor: getRandomColor(),
         tension: 0.1
    },
  ]
  };

  const config = {
    type: 'line',
    data: data,
     options: {
       scales: {
         y: [{
           beginAtZero: true
         }],
         yAxes: [{
            scaleLabel: {
            display: true,
            labelString: 'Measured Voltages (V)'
      }
    }],
    xAxes: [{
            scaleLabel: {
            display: true,
            labelString: 'TimeStamps in (yyyy-mm-dd hh:mm:ss)'
      }
            ,
       ticks: {
               display: false
             }
    }]
       }
     },
  };

  const config_loadCell = {
    type: 'line',
    data: dataLoadCell,
     options: {

       scales: {
                  x: {
            ticks: {
              display: false
            }

         },
         y: {
           beginAtZero: true,
         },


         yAxes: [{
            scaleLabel: {
            display: true,
            labelString: 'Mass in grams (g)'
      }
    }],
    xAxes: [{
            scaleLabel: {
            display: true,
            labelString: 'TimeStamps in (yyyy-mm-dd hh:mm:ss)'
      }
      ,
       ticks: {
               display: false
             }
    }]
       }
     },
  };

  var myChart = new Chart(
    document.getElementById('myChart'),
    config
  );

  var myChart_LoadCell = new Chart(
    document.getElementById('myChart_LoadCell'),
    config_loadCell
  );

  function getRandomColor() {
  var letters = '0123456789ABCDEF';
  var color = '#';
  for (var i = 0; i < 6; i++) {
    color += letters[Math.floor(Math.random() * 16)];
  }
  return color;
}

</script>

</body>
</html>