clear;

dataLens = 3;
dataType = 'single';

s = kSerial(115200, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

getFirstSequenceNum = true;
firstSequenceNum = 0;

tic
for i = 1 : 10000
%while true
    [packetData, packetLens] = s.packetRecv(dataLens, dataType);
    if packetLens > 0
        s.dataBuffer = [s.dataBuffer(:, packetLens + 1 : end), packetData];       % record data
        time = s.dataBuffer(1 : 3, end);
        time(3) = fix(time(3) * 100);
        freq = s.getFreq([1, 2, 3], 128);
        fprintf('[%05i][%02i][%02i:%02i:%02i][%3dHz]\n', s.packet.sequenceNum, packetLens, time, fix(freq));
        if getFirstSequenceNum
            firstSequenceNum    = s.packet.sequenceNum;
            getFirstSequenceNum = false;
        end
    end
end
time = toc;

lostPacket = (s.packet.sequenceNum - firstSequenceNum + 1) - s.packet.packetCount;
sampleRate = s.packet.packetCount / time;
fprintf('\nrecv packet = %d, lost packet = %d, sample rate = %.3f Hz\n', s.packet.packetCount, lostPacket, sampleRate);

s.close();
