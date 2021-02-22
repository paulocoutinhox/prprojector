# PRProjector

[![Build - macOS](https://github.com/paulo-coutinho/prprojector/actions/workflows/macos.yml/badge.svg?branch=master)](https://github.com/paulo-coutinho/prprojector/actions/workflows/macos.yml)
[![Build - Windows x86](https://github.com/paulo-coutinho/prprojector/actions/workflows/win32.yml/badge.svg?branch=master)](https://github.com/paulo-coutinho/prprojector/actions/workflows/win32.yml)
[![Build - Windows x64](https://github.com/paulo-coutinho/prprojector/actions/workflows/win64.yml/badge.svg?branch=master)](https://github.com/paulo-coutinho/prprojector/actions/workflows/win64.yml)

Software to show images, videos, texts and lyrics on datashow or any external monitor and display.

<p align="center"><a href="https://github.com/paulo-coutinho/prprojector" target="_blank" rel="noopener noreferrer"><img src="extras/screenshots/screenshot1.png" alt="screenshot 1"></a></p>

## Download

Get your version on release page.

## Requirements to build

- Python 3

## How to build

1 - Install python requirements:

```
pip install -r requirements.txt
```

2 - Call build for your platform:

```
python make.py run build-macos

or

python make.py run build-linux

or

python make.py run build-windows
```
