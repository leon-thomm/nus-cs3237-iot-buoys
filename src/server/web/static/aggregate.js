function get_aggregate() {
    fetch('/aggregate')
    .then((response) => response.json())
    .then((data) => {
        console.log(data)
        turbulence = 0.0
        for(let i = 0; i < data.turbulence[0].length; i++) {
            turbulence += Math.abs(data.turbulence[0][i])
        }
        turbulence = (turbulence/data.turbulence[0].length).toFixed(6)
        
        light = 0.0
        for(let i = 0; i < data.light[0].length; i++) {
            if(data.light[0][i] > 0.8) {
                val = (Math.random() * (0.03 - 0.01) + 0.01).toFixed(7)
                light += parseFloat(val)
            } else {
                light += (0.8 - data.light[0][i])/0.8
            }
        }
        light = (light/data.light[0].length).toFixed(6)
        
        temp = 0.0
        for(let i = 0; i < data.temp[0].length; i++) {
            temp += data.temp[0][i]
        }
        temp = temp/data.temp[0].length
        temp = (temp - 4.46) * 100
        temp = temp - 0.19
        temp = temp * 100
        
        total = (light * 0.45) + (turbulence * 0.45) + (temp * 0.1)
        score = ((1-total) * 100).toFixed(2)
        console.log(total)
        document.getElementById("deviation").innerHTML = total
        document.getElementById("score").innerHTML = score
    })
}

function initMap() {
    // The location of Uluru
    const com3 = { lat: 1.294528, lng: 103.774525 };
    // The map, centered at Uluru
    const map = new google.maps.Map(document.getElementById("map"), {
      zoom: 18,
      center: com3,
    });
    // The marker, positioned at Uluru
    const marker0 = new google.maps.Marker({
      position: {lat: 1.29486, lng: 103.77441},
      map: map,
    });
    const marker1 = new google.maps.Marker({
      position: {lat: 1.29485, lng: 103.77436},
      map: map,
    });
  }

  window.initMap = initMap;
  get_aggregate()