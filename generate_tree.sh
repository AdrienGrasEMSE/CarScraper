#!/bin/bash

tree -I ".vscode|build|spdlog|uni_algo|.git" > projectTree.txt

echo "Project tree generated in projectTree.txt"