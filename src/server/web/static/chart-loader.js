const ctx_temp = document.getElementById('chart-temp')
const chart_temp = new Chart(ctx_temp, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: "Temperature",
            data: [],
            borderColor: 'rgb(0, 0, 255)',
            tension: 0.1
        }]
    },
    options: {
        responsive: true
    }
})

const ctx_light = document.getElementById('chart-light')
const chart_light = new Chart(ctx_light, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: "Light level",
            data: [],
            borderColor: 'rgb(0, 255, 0)',
            tension: 0.1
        }]
    }
})

const ctx_acc = document.getElementById('chart-acc')
const chart_acc = new Chart(ctx_acc, {
    type: 'line',
    data: {
        labels: [],
        datasets: [
            {
                label: "Accelerometer x-axis",
                data: [],
                borderColor: 'rgb(0,63,92)',
                tension: 0.1
            },
            {
                label: "Accelerometer y-axis",
                data: [],
                borderColor: 'rgb(122,81,149)',
                tension: 0.1
            },
            {
                label: "Accelerometer z-axis",
                data: [],
                borderColor: 'rgb(239,86,117)',
                tension: 0.1
            },
            {
                label: "Accelerometer g-force",
                data: [],
                borderColor: 'rgb(255,166,0)',
                tension: 0.1
            }
        ]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false
    }
}) 

const ctx_gyro = document.getElementById('chart-gyro')
const chart_gyro = new Chart(ctx_gyro, {
    type: 'line',
    data: {
        labels: [],
        datasets: [
            {
                label: "Gyroscope Pitch x-axis",
                data: [],
                borderColor: 'rgb(0,63,92)',
                tension: 0.1
            },
            {
                label: "Gyroscope Yaw",
                data: [],
                borderColor: 'rgb(122,81,149)',
                tension: 0.1
            },
            {
                label: "Gyroscope Roll",
                data: [],
                borderColor: 'rgb(239,86,117)',
                tension: 0.1
            }
        ]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false
    }
}) 

function test_add_data() {
    time.push(242630)
    temp.push(100)
    chart_temp.update();
}

function update() {
    fetch('/mongo/update')
    .then((response) => response.json())
    .then((data) => {
        chart_temp.data.labels = data.time
        chart_temp.data.datasets[0].data = data.temp
        chart_temp.update()

        chart_light.data.labels = data.time
        chart_light.data.datasets[0].data = data.light
        chart_light.update()

        chart_acc.data.labels = data.time
        chart_acc.data.datasets[0].data = data.acc_x
        chart_acc.data.datasets[1].data = data.acc_y
        chart_acc.data.datasets[2].data = data.acc_z
        chart_acc.data.datasets[3].data = data.acc_g
        chart_acc.update()

        chart_gyro.data.labels = data.time
        chart_gyro.data.datasets[0].data = data.gyro_pitch
        chart_gyro.data.datasets[1].data = data.gyro_yaw
        chart_gyro.data.datasets[2].data = data.gyro_roll
        chart_gyro.update()
    })
}

setInterval(update, 2000)