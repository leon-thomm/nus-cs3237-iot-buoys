import pandas as pd
import matplotlib.pyplot as plt

def plot(file):
    df = pd.read_csv(file, engine='python')
    fig, ax1 = plt.subplots(3,2)
    ax1[0,0].plot(df['acc_x'].values, color = 'green', label = 'x')
    ax1[0,0].plot(df['acc_y'].values, color = 'red', label = 'y')
    ax1[0,0].legend()
    ax1[0,1].plot(df['acc_z'].values, color = 'blue', label = 'z')
    ax1[0,1].legend()
    ax1[1,0].plot(df['acc_g'].values, color = 'orange', label = 'g')
    ax1[1,0].legend()
    ax1[1,1].plot(df['gyro_yaw'].values, color = 'green', label = 'roll')
    ax1[1,1].plot(df['gyro_roll'].values, color = 'red', label = 'pitch')
    ax1[1,1].plot(df['gyro_pitch'].values, color = 'blue', label = 'yaw')
    ax1[1,1].legend()
    ax1[2,0].plot(df['temp'].values, label='temp')
    ax1[2,0].legend()
    ax1[2,1].plot(df['light'].values, label='light')
    ax1[2,1].legend()
    plt.show()

def plot_aggregated(file):
    df = pd.read_csv(file, engine='python')
    fig, ax1 = plt.subplots(2,2)
    ax1[0,0].plot(df['turbulence'].values, color = 'green', label = 'turb')
    ax1[0,0].legend()
    ax1[0,1].plot(df['light'].values, color = 'blue', label = 'light')
    ax1[0,1].legend()
    ax1[1,0].plot(df['temp'].values, color = 'orange', label = 'temp')
    ax1[1,0].legend()
    plt.show()

#plot('data.csv')
#plot('data-moderated.csv')
plot_aggregated('data-aggregated.csv')