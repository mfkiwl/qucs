<Qucs Schematic 0.0.3>
<Properties>
  <View=6,-33,783,422,1,0,0>
  <Grid=10,10,1>
  <DataSet=giacoletto.dat>
  <DataDisplay=giacoletto.dpl>
  <OpenDisplay=1>
</Properties>
<Components>
  <L L1 1 130 140 -26 10 0 0 "1.5 nH" 1>
  <R R2 1 260 190 13 -26 0 1 "66.7 Ohm" 1 "26.85" 0 "US" 0>
  <C C1 1 360 190 17 -26 0 1 "38.82 pF" 1>
  <R R1 1 210 140 -26 13 0 0 "45 Ohm" 1 "26.85" 0 "US" 0>
  <GND * 1 360 240 0 0 0 0>
  <GND * 1 60 240 0 0 0 0>
  <VCCS SRC1 1 550 190 -26 34 0 0 "1.2 S" 1 "0" 1>
  <L L2 1 630 140 -26 10 0 0 "1.5 nH" 1>
  <GND * 1 680 240 0 0 0 0>
  <C C2 1 470 80 -26 17 0 0 "0.1 pF" 1>
  <C C3 1 310 60 -26 17 0 0 "0.65 pF" 1>
  <Pac P2 1 60 190 18 -26 0 1 "1" 1 "50 Ohm" 1 "0 dBm" 0 "1 GHz" 0>
  <Pac P1 1 680 190 18 -26 0 1 "2" 1 "50 Ohm" 1 "0 dBm" 0 "1 GHz" 0>
  <.SP SP1 1 160 300 -50 35 0 0 "lin" 1 "100 MHz" 1 "1.1 GHz" 1 "110" 1 "no" 0 "1" 0 "2" 0>
</Components>
<Wires>
  <260 140 360 140 "" 0 0 0>
  <260 140 260 160 "" 0 0 0>
  <360 140 360 160 "" 0 0 0>
  <60 140 60 160 "" 0 0 0>
  <60 220 60 240 "" 0 0 0>
  <60 240 260 240 "" 0 0 0>
  <260 220 260 240 "" 0 0 0>
  <260 240 360 240 "" 0 0 0>
  <360 220 360 240 "" 0 0 0>
  <240 140 260 140 "" 0 0 0>
  <580 140 600 140 "" 0 0 0>
  <580 140 580 160 "" 0 0 0>
  <660 140 680 140 "" 0 0 0>
  <680 140 680 160 "" 0 0 0>
  <680 220 680 240 "" 0 0 0>
  <580 240 680 240 "" 0 0 0>
  <580 220 580 240 "" 0 0 0>
  <360 240 520 240 "" 0 0 0>
  <520 220 520 240 "" 0 0 0>
  <360 140 520 140 "" 0 0 0>
  <520 140 520 160 "" 0 0 0>
  <500 80 580 80 "" 0 0 0>
  <580 80 580 140 "" 0 0 0>
  <360 80 360 140 "" 0 0 0>
  <360 80 440 80 "" 0 0 0>
  <340 60 600 60 "" 0 0 0>
  <600 60 600 140 "" 0 0 0>
  <160 140 170 140 "" 0 0 0>
  <170 140 180 140 "" 0 0 0>
  <170 60 170 140 "" 0 0 0>
  <170 60 280 60 "" 0 0 0>
  <60 140 100 140 "" 0 0 0>
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
  <Text 60 20 16 #000000 0 "Modified Giacoletto equivalent circuit ">
</Paintings>