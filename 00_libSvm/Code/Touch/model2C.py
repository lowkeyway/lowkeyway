# -*- coding: utf-8 -*-
# @Time    : 6/27/2021 4:50 PM
# @Author  : lowkeyway
import sys
from pathlib import Path
import re
import numpy as np

model_path = "./out/touch_heart_scale.model"
h_file = "./svm_model.h"

svm_type = {"c_svc":"0"}
kernel_type = {"rbf":"2"}

class model:
    def __init__(self):
        self.log_path = None

        self.svm_type = None
        self.kernel_type = None
        self.gamma = None
        self.nr_class = None
        self.total_sv = None
        self.rho = []
        self.label = []
        self.nr_sv = []
        self.sv_coef = []
        self.sv_data = []



    def load(self, log_path):
        self.log_path = Path(log_path).resolve(True)
        print(self.log_path)

        if self.log_path.is_file():
            try:
                file_name = self.log_path.name
                print("File_name:\n", file_name)
                data_begain = 0
                with self.log_path.open() as f:
                    for line in f:
                        data = line.split()
                        if "svm_type" == data[0]:
                            self.svm_type = svm_type[data[1]]
                        elif "kernel_type" == data[0]:
                            self.kernel_type = kernel_type[data[1]]
                        elif "gamma" == data[0]:
                            self.gamma = data[1]
                        elif "nr_class" == data[0]:
                            self.nr_class = data[1]
                        elif "total_sv" == data[0]:
                            self.total_sv = data[1]
                        elif "rho" == data[0]:
                            for l in data[1:]:
                                self.rho.append(l)
                        elif "label" == data[0]:
                            for l in data[1:]:
                                self.label.append(l)
                        elif "nr_sv" == data[0]:
                            for n in data[1:]:
                                self.nr_sv.append(n)
                        elif "SV" == data[0]:
                            data_begain = 1
                        else:
                            if data_begain:
                                node_temp = []
                                self.sv_coef.append(data[0])
                                for SV in data[1:]:
                                    node = SV.split(":")
                                    node_temp.append(node[1])
                                self.sv_data.append(node_temp)
                            else:
                                print("--", line)

            except Exception:
                print(f"[INVALID LOG FILE]{self.log_path.name}")

    def save(self, file_path):
        svm_model_h = ""
        with open(file_path, mode='w', encoding='utf-8') as file_obj:
            svm_model_h += "//This file is generated by model2C.py, DO NOT modify it\n"
            svm_model_h += "#define SVM_TYPE " + self.svm_type + "\n"
            svm_model_h += "#define KERNEL_TYPE " + self.kernel_type + "\n"
            svm_model_h += "#define GAMMA " + self.gamma + "\n"
            svm_model_h += "#define NR_CLASS " + self.nr_class + "\n"
            svm_model_h += "#define TOTAL_SV " + self.total_sv + "\n"

            svm_model_h += "double rho[%d] = {" %(len(self.rho))
            for enum in self.rho:
                svm_model_h += enum + ", "
            svm_model_h += "};\n"

            svm_model_h += "int label[%d] = {" %(len(self.label))
            for enum in self.label:
                svm_model_h += enum + ", "
            svm_model_h += "};\n"

            svm_model_h += "int nr_sv[%d] = {" % (len(self.nr_sv))
            for enum in self.nr_sv:
                svm_model_h += enum + ", "
            svm_model_h += "};\n"

            svm_model_h += "double sv_coef[1][%d] = {\n\t{" % (len(self.sv_coef))
            for i, enum, in enumerate(self.sv_coef):
                if 0 == i %10:
                    svm_model_h += "\n\t"
                svm_model_h += enum + ", "

            svm_model_h += "\n\t},\n};\n"

            # svm_model_h += "struct svm_node SV[%d][%d] = {" % (len(self.sv_data), len(self.sv_data[0]) + 1)
            # for i, enum, in enumerate(self.sv_data):
            #     svm_model_h += "\n\t{"
            #     for j, data in enumerate(enum):
            #         if 0 == j %10:
            #             svm_model_h += "\n\t\t"
            #         svm_model_h += "{%d, %s}, " %(j+1, data)
            #
            #     svm_model_h += "{-1, -1}"
            #     svm_model_h += "\n\t},\n"
            #     # if 0 == i % 10:
            #     #     svm_model_h += "\n\t"
            # svm_model_h += "\n};\n"

            svm_model_h += "double SV[%d][%d] = {" % (len(self.sv_data), len(self.sv_data[0]))
            for i, enum, in enumerate(self.sv_data):
                svm_model_h += "\n\t{"
                for j, data in enumerate(enum):
                    if 0 == j % 8:
                        svm_model_h += "\n\t\t"
                    svm_model_h += "%8s, " %(data)
                svm_model_h += "\n\t},\n"
                # if 0 == i % 10:
                #     svm_model_h += "\n\t"
            svm_model_h += "\n};\n"

            file_obj.write(svm_model_h)
            # for h in self.head_list:
            #     file_obj.write(h + "\n")

    def dump(self):
        print(self.log_path)
        print(self.svm_type)
        print(self.kernel_type)
        print(self.gamma)
        print(self.nr_class)
        print(self.total_sv)
        print(self.rho)
        print(self.label)
        print(self.nr_sv)
        print(self.sv_coef)
        print(self.sv_data)

def main_func(argv):
    args = len(argv)
    if args == 1:
        log_path = model_path
    elif args == 2:
        log_path = argv[1]
    else:
        print("Incorrect Input!\n")
        return

    print("log_path = ", log_path)

    M = model()
    M.load(log_path)
    M.dump()
    M.save(h_file)



if __name__ == '__main__':
    main_func(sys.argv)