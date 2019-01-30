import matplotlib.pyplot as plt
import re
import math


def get_data(file):
    # Returns sizes of cache, miss rates.
    sizes = []
    miss_rates = []
    with open(file, 'r') as f:
        for line in f:
            if "Cache size" in line:
                beg = re.search(r"\d", line).start()
                end = line.index("B")
                sizes.append(math.log(int(line[beg:end]), 2))
            if "Cache Hit Rate" in line:
                beg = re.search(r"\d", line).start()
                end = line.index("%")
                miss_rates.append(100.00-float(line[beg:end]))
    return sizes, miss_rates


def get_wb_wt(file):
    # Returns sizes of cache, writeback and writethrough data.
    sizes = []
    wb = []
    wt = []
    with open(file, 'r') as f:
        for line in f:
            if "Cache size" in line:
                beg = re.search(r"\d", line).start()
                end = line.index("B")
                sizes.append(math.log(int(line[beg:end]), 2))
            elif "write-back" in line:
                beg = re.search(r"\d", line).start()
                wb.append(int(line[beg:]))
            elif "write-thru" in line:
                beg = re.search(r"\d", line).start()
                wt.append(int(line[beg:]))

    return sizes, wb, wt


# Plots sizes vs miss rates.
def plot_miss(x, y):
    plt.plot(x, y, linewidth=3)
    plt.xlabel("Cache Capacity (2^x Bytes)")
    plt.ylabel("Cache Miss Rate (%)")
    plt.title("Cache Size vs. Miss Rate")
    plt.show()


def plot_data_wb_wt(wb, wt, sizes):
    plt.plot(sizes, wb, linewidth=3)
    plt.plot(sizes, wt, linewidth=3)
    plt.legend(["Writeback data", "Writethrough data"])
    plt.title("Cache Size vs. Writeback/Writethrough Data")
    plt.xlabel("Cache Capacity (2^x Bytes)")
    plt.ylabel("Bytes")
    plt.show()


if __name__ == "__main__":
    sizes, wb, wt = get_wb_wt("log.txt")
    sizes = [3, 4, 5, 6, 7, 8, 9]
    plot_data_wb_wt(wb, wt, sizes)
    sizes, miss_rates = get_data("log.txt")
    sizes = [3, 4, 5, 6, 7, 8, 9]
    plot_miss(sizes, miss_rates)
