#!/bin/bash

tree -I ".vscode|build|spdlog|uni_algo|.git|input_excel|save_html|test_save_html" > projectTree.txt

echo "Project tree generated in projectTree.txt"