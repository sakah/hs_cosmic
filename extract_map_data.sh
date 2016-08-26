#LayerLength 
#layer/I 
#wire/I 
#xhv 
#yhv 
#x0 
#y0 
#xro 
#yro 
#LayerID/I 
#isSenseWire/I 
#CellID/I 
#BoardID/I 
#BrdLayID/I 
#BrdLocID/I 
#ChanID/I

awk 'NR>=2 {print $13,$16,$10,0,$12,$4,$5,$6,$7,$8,$9}' chanmap_20160814.txt | grep -v "^-1"  | sort -n -k 1,1 -k 2,2 > wirepos_20160814.txt
