import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D


# Obtiene los datos

data = np.loadtxt("datos.dat")
data = data.T
# Descifra el formato
x_t = [] # Los valores de densidad de probabilidad
z_t = [] # Los valores de posicion
for i in range(len(data)/2):
    x_t.append(data[2*i+1])
    z_t.append(data[2*i][0])
    

x_t = np.array(x_t).T
z_t = np.array(z_t).T
print len(z_t), len(x_t[0])

########################
# Grafica la animacion #
########################

fig, ax = plt.subplots()

# Selecciona los limites de la grafica 
lim = 80

y = x_t[0]        # y-array
x = z_t        # x-array
#ax.set_ylim((0,limy))
#ax.set_xlim((-limMinz,limMaxz))
ax.set_ylim((0,1))
ax.set_xlim((-lim,lim))
line, = ax.plot(x, y)

def animate(i):
    #i = (i)%len(x_t)
    i = i%len(x_t)
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
