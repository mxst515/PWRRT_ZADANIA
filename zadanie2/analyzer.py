import numpy as np
import matplotlib.pyplot as plt

t1, x1, y1 = np.loadtxt('./sensor1.csv', delimiter=',', skiprows=1, unpack=True)
t2, x2, y2 = np.loadtxt('./sensor2.csv', delimiter=',', skiprows=1, unpack=True)
# t3, x3, y3 = np.loadtxt('./ttt2.csv', delimiter=',', skiprows=1, unpack=True)
# t4, x4, y4 = np.loadtxt('./test3.csv', delimiter=',', skiprows=1, unpack=True)
t5, x5, y5 = np.loadtxt('./test4.csv', delimiter=',', skiprows=1, unpack=True)

fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(13, 4))

ax1.plot(x1, y1, color='blue', label='sens1')
ax1.plot(x2, y2*-1, color='red', label='sens2')
ax1.plot(x5, y5, color='black', label='final')
ax1.set_title('widok z gory')
ax1.set_xlabel('X (przod/tyl)')
ax1.set_ylabel('Y (prawo/lewo)')
ax1.grid(True)
ax1.legend()

ax2.plot(t1, x1, color='blue', label='sens 1')
ax2.plot(t2, x2, color='red', label='sens 2')
ax2.plot(t5, x5, color='black', label='final')
ax2.set_title('sens.x w czasie')
ax2.set_xlabel('time')
ax2.set_ylabel('X')
ax2.grid(True)
ax2.legend()

ax3.plot(t1, y1, color='blue', label='sens 1')
ax3.plot(t2, y2*-1, color='red', label='sens 2')
# ax3.plot(t4, y4, color='green', label='ttt')
ax3.plot(t5, y5, color='black', label='final')
ax3.set_title('sens.y w czasie')
ax3.set_xlabel('time')
ax3.set_ylabel('Y')
ax3.grid(True)
ax3.legend()

plt.tight_layout()
plt.show()