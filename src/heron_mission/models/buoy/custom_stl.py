import numpy as np


R = np.array([[+1, 0, 0],
              [0, 0, -1],
              [0, +1, 0]])
S = 3e-3 * np.eye(3)


if __name__ == '__main__':
    f_in = open('buoy_ascii.stl', 'r')
    lines = f_in.readlines()
    f_in.close()

    f_out = open('buoy_custom.stl', 'w')

    for line in lines:
        if ('normal' in line) or ('vertex' in line):
            tokens = line.split()
            header = tokens[:-3]
            vector = np.array([float(x) for x in tokens[-3:]]).reshape(3, 1)

            new_vec = R @ vector
            if 'vertex' in line:
                new_vec = S @ new_vec
            tail = [str(round(x, 6)) for x in new_vec.flatten()]
            new_line = ' ' + ' '.join(header + tail) + '\n'
            if 'vertex' in line:
                new_line = ' ' + new_line

            f_out.write(new_line)
        else:
            f_out.write(line)

    f_out.close()
