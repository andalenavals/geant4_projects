#/bin/bash
unset G4ANALYSIS_USE
MAXPARALLEL=7
make clean
if make
then
    echo "DONE"
else
    exit
fi
rm -f long_run.mac
declare -i depth=0 Mev=0
depth=0
for Mev in $(seq 200 -10 10)
do 
    for height in $(seq 0 1 6)
    do
	new_macro=long_run${Mev}Mev_${depth}cm_${height}cm.mac
	sed 's/{height}/'${height}'/g' macros/long_run.mac > ${new_macro}
	sed -i 's/{Mev}/'${Mev}'/g' ${new_macro}
	echo $new_macro
	PICompton ${new_macro} &>/dev/null &
	PID=$!
	if [ $(jobs -p|wc -l) -ge "$MAXPARALLEL" ]; then
	    wait $PID
	fi
    done
done
rm -f long_run*.mac
wait