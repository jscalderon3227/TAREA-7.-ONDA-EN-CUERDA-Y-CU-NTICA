import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D


# Obtiene los datos

data = np.loadtxt("Wave_Eq.dat")
x_t = data

########################
# Grafica la animacion #
########################

fig, ax = plt.subplots()

# Selecciona los limites de la grafica
limx = np.max(np.arange(len(x_t[0])),0)
maxims = np.array([np.max(x_t[j],0) for j in range(len(x_t))])
limy = np.max(maxims,0)

y = x_t[0]        # x-array
x = np.arange(len(x_t[0]))
ax.set_ylim((-limy,limy))
ax.set_xlim((0,limx))
line, = ax.plot(x, y)

def animate(i):
    #i = (i)%len(x_t)
    i = 2*i%len(x_t)
    line.set_ydata(x_t[i])  # update the data
    return line,

#Init only required for blitting to give a clean slate.
def init():
    line.set_ydata(np.ma.array(x, mask=True))
    return line,

ani = animation.FuncAnimation(fig, animate,  init_func=init,
    interval= 1, frames = 4000, blit = True)

plt.show()
plt.close()
