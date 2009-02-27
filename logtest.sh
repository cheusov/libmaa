#!/bin/sh

rm _log_file.txt

env LC_ALL=C ./logtest _log_file.txt
awk '
{
   sub(/Mon|Tue|Wed|Thu|Fri|Sat|Sun/, "Day")
   sub(/Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec/, "Mth")
   sub(/\[[0-9]+\]/, "[NNNNN]")
   gsub(/[0-9][0-9]/, "NN")
   print
}' _log_file.txt
