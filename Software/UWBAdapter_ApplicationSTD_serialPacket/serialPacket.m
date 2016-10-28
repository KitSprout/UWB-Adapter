clear;

dataLens = 3;
dataType = 'single';

s = kSerial(256000, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

getFirstRecord = true;
firstSequenceNum = 0;

tic
for i = 1 : 10000
%while true
    [packetData, packetLens] = s.packetRecv(dataLens, dataType);
    if packetLens > 0
        s.dataBuffer = [s.dataBuffer(:, packetLens + 1 : end), packetData];       % record data
        time = s.dataBuffer(1 : 3, end);
        time(3) = fix(time(3) * 100);
        freq = s.getFreq([1, 2, 3], 256);
        fprintf('[%05i][%02i][%02i:%02i:%02i][%3dHz]\n', s.packet.sequenceNum, packetLens, time, fix(freq));
        if getFirstRecord
            getFirstRecord = false;
            firstSequenceNum = s.packet.sequenceNum - packetLens + 1;
        end
    end
end
systic = toc;

totalPacket = s.packet.sequenceNum - firstSequenceNum + 1;
lostPacket  = totalPacket - s.packet.packetCount;
sampleRate  = s.packet.packetCount / systic;
fprintf('\nrecv packet = %d, lost packet = %d, sampleRate = %d Hz\n', s.packet.packetCount, lostPacket, fix(sampleRate + 0.5));

s.close();
