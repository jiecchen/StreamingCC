#!/bin/bash
cat $@ | column -t -s, | less -S