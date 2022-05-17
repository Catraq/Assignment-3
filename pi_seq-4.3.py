import numpy as np;
import matplotlib.pyplot as plt;

size = np.array([8, 16, 32, 64, 128]);
time2 = np.array([3.579621, 3.171401, 0.942964, 0.562067, 0.301035]);
time = np.array([3.550858 , 3.141643, 0.895148, 0.629637 , 0.333692]);


#time_approx = np.array([3.579621, 3.579621/2, 3.579621/4, 3.579621/8, 3.579621/16]);

plt.plot(size, time, size, time2); #, size, time_approx);
plt.legend(["Non-blocking", "Blocking"])
plt.ylabel("Time[S]");
plt.xlabel("Processes[N]");
plt.show();
