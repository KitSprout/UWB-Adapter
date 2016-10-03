clear;

dataSize = 4;
s = kSerial(256000, 'clear');
s.dataBuffer = zeros(dataSize, 1024 * 8);
s.open();

getFirstSequenceNum = true;
firstSequenceNum = 0;

tic
for i = 1 : 10000
%while true
    [packetData, packetLens] = s.packetRecv(dataSize, 'single');
    if packetLens > 0
        s.dataBuffer = [s.dataBuffer(:, packetLens + 1 : end), packetData];       % record data
        leastSequenceNum = s.dataBuffer(end);
        fprintf('[%5i][%2i][%8.3f, %8.3f, %8.3f]\n', leastSequenceNum, packetLens, s.dataBuffer(1 : end - 1, end));
        if getFirstSequenceNum
            firstSequenceNum    = leastSequenceNum;
            getFirstSequenceNum = false;
        end
    end
end
time = toc;

lostPacket = (leastSequenceNum - firstSequenceNum + 1) - s.packet.packetCount;
sampleRate = s.packet.packetCount / time;
fprintf('\nrecv packet = %d, lost packet = %d, sample rate = %.3f Hz\n', s.packet.packetCount, lostPacket, sampleRate);

s.close();
