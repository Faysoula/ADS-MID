cmake_minimum_required(VERSION 3.28)
project(ADS_midterm_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(ADS_midterm_project main.cpp
        ForestTree.cpp
        ForestTree.h
        Account.h
        TreeNode.cpp
        TreeNode.h
        Transaction.cpp
        Transaction.h
        Account.cpp
)
