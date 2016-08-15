#!/bin/bash
#update tags list
ctags -f my_tags -R

#list for other directories
:set tags+=/usr/lib/avr/include
