# -*- coding: utf-8 -*-
# @Time    : 6/19/2021 12:35 PM
# @Author  : lowkeyway
import sys
import os, shutil
# os.chdir("./windows")
from python.svmutil import *
from python.grid import *

train_path = "./raw/"
out_path = "./out/"
win_path = "./windows/"
touch_log_scale = train_path + "touch_heart_scale"
touch_log_model = out_path + "touch_heart_scale.model"
touch_log_model_win = win_path + "touch_heart_scale.model"

def main_func(argv):
    rate, param = find_parameters(touch_log_scale)

    print("\n@@@ The best -c is %f, the best -g is %f @@@\n" %(param["c"], param["g"]))
    y, x = svm_read_problem(touch_log_scale)
    # m = svm_train(y, x)
    m = svm_train(y, x, "-c %f -g %f" %(param["c"], param["g"]))
    svm_save_model(touch_log_model, m)
    shutil.copyfile(touch_log_model, touch_log_model_win)


if __name__ == '__main__':
    main_func(sys.argv)