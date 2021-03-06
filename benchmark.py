import subprocess
import os
import errno

size_list = [16, 64, 128, 256, 1024, 2048]
count_list = [1e3, 1e4, 1e5, 1e6]


def benchmark(prog):
    result = ""
    for count in count_list:
        for size in size_list:
            data_size = size * count / 1e6
            cp = subprocess.run([prog, str(size), str(count)], stdout=subprocess.PIPE)
            p_output = cp.stdout.decode("utf8")
            result += "Data size: {} megabytes.\tChunk size: {} bytes.\tTime cost: {} seconds.\n\n".format(str(data_size), str(size), str(p_output))
    return result



pipe_result = benchmark("./pipe.out")
msgq_result = benchmark("./msgq.out")

output_dir = "./output/"
if not os.path.exists(output_dir):
    try:
        os.makedirs(output_dir)
    except OSError as exc: # Guard against race condition
        if exc.errno != errno.EEXIST:
            raise

# Write to output file
with open(output_dir + "pipe.txt", "w") as f:
    f.write(pipe_result)
    f.close()

with open(output_dir + "msgq.txt", "w") as f:
    f.write(msgq_result)
    f.close()