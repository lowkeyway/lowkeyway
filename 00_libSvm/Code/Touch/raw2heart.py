# -*- coding: utf-8 -*-
# @Time    : 6/15/2021 1:11 PM
# @Author  : lowkeyway
import sys
from pathlib import Path
import re
import numpy as np

log_path = "./raw"
normal_log = "Normal"
fd_log = "FD"
out_path = "./out/"
heart_log = out_path  + "touch_heart"
heart_scale_log = out_path + "touch_heart_scale"
heart_range_log = out_path + "touch_heart_range"
FD_LINE = 10
SCALE_MIN = -1
SCALE_MAX = 1


class raw:
    def __init__(self):
        self.row = None
        self.col = None
        self.data = None
        self.label = None

    def __init__(self, raw_data, label = -1):
        self.row = int(raw_data[0])
        self.col = int(raw_data[1])
        self.data = np.array([x.rstrip(',') for x in raw_data[2].split()], dtype=np.int32).reshape(self.row, self.col)
        self.label = label

        # print(self.row)
        # print(self.col)
        # print(self.data)


class heart:
    def __init__(self):
        self.label = None
        self.data = None

    def __init__(self, label, data):
        self.label = str(label)
        self.data = ""
        data = data.reshape(1, -1)[0]
        for index, element in enumerate(data):
            self.data += str(index+1)
            self.data += ":"
            self.data += str(element)
            self.data += " "

    def show(self):
        print(self.label, " ", self.data)


class raw2heart:
    def __init__(self):
        self.log_path = None
        self.raw_list = []
        self.label_list = []
        self.head_list = []
        self.feature_max = []
        self.feature_min = []

    def load(self, log_path):
        self.log_path = Path(log_path).resolve(True)
        self.raw = self.load_raw()


    def load_raw(self):
        print(f"loading raw logs from \"{self.log_path}\"")

        logs = []

        for entry in self.log_path.glob("*.txt"):
            label = -1
            print("Entry:\n", entry)
            if entry.is_file():
                try:
                    file_name = entry.name
                    print("File_name:\n", file_name)

                    if fd_log in file_name:
                        label = 1
                    print("label = ", label)

                    text = entry.read_text()
                    data = re.findall(r'BEGIN_UUT.*?Dmax = (.*?)\nSmax = (.*?)\n(.*?)Min:.*?Max:.*?Average:.*?END_UUT',
                                      text, re.DOTALL)
                    # print("Data", data)

                    for raw_data in data:
                        self.label_list.append(label)
                        # print("raw_data: \n", raw_data)
                        r = raw(raw_data, label)
                        self.raw_list.append(r)
                        h = heart(label, r.data[0:FD_LINE])
                        # h.show()
                        h_line = h.label + " " + h.data
                        self.head_list.append(h_line)
                except Exception:
                    print(f"[INVALID LOG FILE]{entry.name}")

    def show_raw(self):
        for r in self.raw_list:
            print("label: [%d]\n" %(r.label), r.data)

    def show_heart(self):
        for h in self.head_list:
            print(h)

    def save_heart(self, file_path):
        with open(file_path, mode='w', encoding='utf-8') as file_obj:
            for h in self.head_list:
                file_obj.write(h + "\n")

    def scale_heart(self, scale_path = None, range_path = None):
        if scale_path is None:
            print("scale_path is None\n")
            return

        if range_path is None:
            range_path = scale_path + "_range"

        original_data = []
        scale_data = []

        for raw in self.raw_list:
            original_data.append(raw.data[0:FD_LINE].reshape(1, -1)[0])

        original_data = np.array(original_data)
        # print("original_data : \n", original_data)

        for index in range(len(original_data[0])):
            self.feature_max.append(np.max(original_data[:,index]))
            self.feature_min.append(np.min(original_data[:,index]))

        print("feature_max: \n", self.feature_max)
        print("feature_min: \n", self.feature_min)

        self.save_range(range_path)

        for original_line_data in original_data:
            scale_line_data = []
            for index, value in enumerate(original_line_data):
                if value == self.feature_min[index]:
                    value = SCALE_MIN
                elif value == self.feature_max[index]:
                    value = SCALE_MAX
                else:
                    value = SCALE_MIN + (SCALE_MAX - SCALE_MIN) * \
                            (value - self.feature_min[index]) / \
                            (self.feature_max[index] - self.feature_min[index])
                scale_line_data.append(value)
            scale_data.append(scale_line_data)
        print("scale_data: \n", scale_data)

        self.save_heart_scale(heart_scale_log, self.label_list, scale_data)

    def save_range(self, file_path):
        with open(file_path, mode='w', encoding='utf-8') as file_obj:
            file_obj.write("x\n")
            file_obj.write(str(SCALE_MIN) + " " + str(SCALE_MAX) + "\n")
            for index in range(len(self.feature_max)):
                file_obj.write(str(index + 1) + " " + str(self.feature_min[index]) + " " + str(self.feature_max[index]) + "\n")

    def save_heart_scale(self, file_path, label_list, scale_data):
        with open(file_path, mode='w', encoding='utf-8') as file_obj:
            for index in range(len(label_list)):
                file_obj.write(str(label_list[index]) + " ")
                for data_index, data in enumerate(scale_data[index]):
                    file_obj.write(str(data_index + 1) + ":" + format(data, ".<6f") + " ")
                file_obj.write("\n")

def main_func(argv):
    r2h = raw2heart()
    r2h.load(log_path)
    # r2h.show_heart()
    r2h.save_heart(heart_log)
    r2h.scale_heart(heart_scale_log)

if __name__ == '__main__':
    main_func(sys.argv)