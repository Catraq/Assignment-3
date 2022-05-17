import numpy as np;
import matplotlib.pyplot as plt;

size = np.array([8, 16, 32, 64, 128]);
time = np.array([3.579621, 3.171401, 0.942964, 0.562067, 0.301035]);

time_approx = np.array([3.579621, 3.579621/2, 3.579621/4, 3.579621/8, 3.579621/16]);

plt.plot(size, time, size, time_approx);
plt.legend(["Actual", "Scaling"])
plt.ylabel("Time[S]");
plt.xlabel("Processes[N]");
plt.show();
