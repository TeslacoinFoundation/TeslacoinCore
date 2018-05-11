## Teslacoin
### The decentralised digital currency inspired by the great inventor Nikola Tesla created to crowdfund Free Energy development


+ Ticker : TES
+ Max Coins : 100,000,000 TES
+ Algorithm : Hybrid PoW/PoS (PoW Ended)
+ PoS currency growth per year (until hardfork v.4.0) : 12%
+ PoS currency growth per year (after hardfork v.4.0) : 
+ - First year -> 7% (0,5% for Foundation)
+ - Second year -> 6% (0,5% for Foundation)
+ - Third year -> 5% (0,5% for Foundation)
+ - Fouth year -> 4% (0,5% for Foundation)
+ - Unitl achiving max coins -> 3% (0,5% for Foundation)
+ The 0.5% interest sent directly to the Foundation will be destined for funding the TeslaStarter platform expenses, committee members
salaries and to crowdfund projects.
+ Max stake age : 90 days
+ Min stake age : 24 hrs
+ Timings of block (in seconds) : 30
+ P2P Port : 1856
+ RPC Port : 1857



### Building Teslacoind (daemon) and TeslaCoin-Qt (wallet/GUI)

#### 1. Install dependencies and download source code
```
sudo apt-get update
sudo apt-get install git
sudo apt-get install build-essential libtool autotools-dev autoconf pkg-config libssl-dev libboost-all-dev libminiupnpc-dev git libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev libdb++-dev qt4-qmake 
sudo apt-get install libqt4-dev libqrencode-dev libprotobuf-dev protobuf-compiler libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools automake libevent-dev bsdmainutils libdb4.8-dev libdb4.8++-dev
sudo apt-get install software-properties-common
sudo add-apt-repository ppa:bitcoin/bitcoin
sudo apt-get update
git clone https://github.com/TeslacoinFoundation/Teslacoin-v.4.0.git
sudo chmod 777 Teslacoin
```

#### 2. Build Teslacoind (daemon)
```
goto <installDirectory>/Teslacoin/src
cp makefile.unix Makefile
make
./Teslacoind &
```

#### 3. Build TeslaCoin-Qt (wallet/GUI)
```
goto <installDirectory>/Teslacoin
qmake 
make -j 4 
./TeslaCoin-Qt &
```
