#!/bin/bash

tree -I ".vscode|.git|build|data|docs|Doxyfile|generate_tree.sh|libs|logs|projectTree.txt" > projectTree.txt

echo "Project tree generated in projectTree.txt"