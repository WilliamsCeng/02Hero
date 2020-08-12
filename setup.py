import os
import shutil
from distutils.dir_util import copy_tree
from setuptools import find_packages, setup

# global variables
board = os.environ['BOARD']
repo_board_folder = f'boards/{board}'
board_notebooks_dir = os.environ['PYNQ_JUPYTER_NOTEBOOKS']
hw_data_files = []


# check whether board is supported
def check_env():
    if not os.path.isdir(repo_board_folder):
        raise ValueError("Board {} is not supported.".format(board))
    if not os.path.isdir(board_notebooks_dir):
        raise ValueError(
            "Directory {} does not exist.".format(board_notebooks_dir))


# copy overlays to python package
#def copy_overlays():
#    src_ol_dir = os.path.join(repo_board_folder, 'bitstream')
#    dst_ol_dir = os.path.join('AES_En_De_PYNQ', 'bitstream')
#    copy_tree(src_ol_dir, dst_ol_dir)
#    hw_data_files.extend(
#        [os.path.join("..", dst_ol_dir, f) for f in os.listdir(dst_ol_dir)])


# copy notebooks to jupyter home
def copy_notebooks():
    src_nb_dir = os.path.join(repo_board_folder, 'AES_En_De_PYNQ')
    dst_nb_dir = os.path.join(board_notebooks_dir, 'AES_En_De_PYNQ')
    if os.path.exists(dst_nb_dir):
        shutil.rmtree(dst_nb_dir)
    copy_tree(src_nb_dir, dst_nb_dir)


check_env()
copy_notebooks()

setup(
    name="AES_En_De_PYNQ",
    version='1.0',
    url='https://github.com/WilliamsCeng/02Hero.git',
    license='BSD 3-Clause License',
    author="Williams Ceng",
    author_email="",
    packages=find_packages(),
    package_data={
        '': hw_data_files,
    },
    description="PYNQ-AES-En/Decryption System On_PYNQ-Z2"
)