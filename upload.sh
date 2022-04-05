PORT=/dev/ttyUSB0

echo '... compile & upload'
echo '... target:' ${PORT}
arduino-cli compile \
--fqbn arduino:avr:nano:cpu=atmega328old \
-u -p $PORT
echo '... done'