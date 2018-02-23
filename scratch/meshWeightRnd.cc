/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Kirill Andreev <andreev@iitp.ru>
 *
 *
 * By default this script creates m_xSize * m_ySize square grid topology with
 * IEEE802.11s stack installed at each node with peering management
 * and HWMP protocol.
 * The side of the square cell is defined by m_step parameter.
 * When topology is created, UDP ping is installed to opposite corners
 * by diagonals. packet size of the UDP ping and interval between two
 * successive packets is configurable.
 * 
 *  m_xSize * step
 *  |<--------->|
 *   step
 *  |<--->|
 *  * --- * --- * <---Ping sink  _
 *  | \   |   / |                ^
 *  |   \ | /   |                |
 *  * --- * --- * m_ySize * step |
 *  |   / | \   |                |
 *  | /   |   \ |                |
 *  * --- * --- *                _
 *  ^ Ping source
 *
 *  See also MeshTest::Configure to read more about configurable
 *  parameters.
 */


#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mesh-module.h"
#include "ns3/mobility-module.h"
#include "ns3/mesh-helper.h"
#include "ns3/trace-helper.h"

#include "ns3/basic-energy-source.h"
#include "ns3/wifi-radio-energy-model.h"
#include "ns3/basic-energy-source-helper.h"
#include "ns3/wifi-radio-energy-model-helper.h"
#include "ns3/energy-source-container.h"
#include "ns3/device-energy-model-container.h"


//added lib
#include "ns3/gnuplot.h"
#include "ns3/flow-monitor-module.h"
#include <ns3/flow-monitor-helper.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TestMeshScript");

Ptr<BasicEnergySource> es;
typedef std::vector<Ptr<BasicEnergySource> > esList;
esList m_esList;

class MeshTest
{
public:
    void ReadRemainingEnergy();
  /// Init test
  MeshTest ();
  /// Configure test from command line arguments
  void Configure (int argc, char ** argv);
  /// Run test
  int Run ();
private:
  int       m_xSize;
  int       m_ySize;
  double    m_step;
  double    m_randomStart;
  double    m_totalTime;
  double    m_packetInterval;
  uint16_t  m_packetSize;
  uint32_t  m_nIfaces;
  bool      m_chan;
  bool      m_pcap;
  std::string m_stack;
  std::string m_root;
  /// List of network nodes
  NodeContainer nodes;
  /// List of all mesh point devices
  NetDeviceContainer meshDevices;
  //Addresses of interfaces:
  Ipv4InterfaceContainer interfaces;
  // MeshHelper. Report is not static methods
  MeshHelper mesh;
  int m_RANDOM_SEED;
  int m_level;
private:
  /// Create nodes and setup their mobility
  void CreateNodes ();
  /// Install internet m_stack on nodes
  void InstallInternetStack ();
  /// Install applications
  void InstallApplication ();
  /// Print mesh devices diagnostics
  void Report ();
};
MeshTest::MeshTest () :
  m_xSize (4),
  m_ySize (4),
  m_step (100.0),
  m_randomStart (0.01),
  m_totalTime (601.0),
  m_packetInterval (0.02),
  m_packetSize (160),
  m_nIfaces (1),
  m_chan (true),
  m_pcap (false),
  m_stack ("ns3::Dot11sStack"),
  m_root ("00:00:00:00:00:01")
{
}

void MeshTest::ReadRemainingEnergy() {
	Time nextread;
	double remainingE;
	std::cout << "------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	for (esList::const_iterator i = m_esList.begin(); i != m_esList.end(); i++) {
		Ptr<BasicEnergySource> es = (*i);
		remainingE = es->GetRemainingEnergy();
		std::cout << "remeining energy at time : " << Simulator::Now() << " at node " << es->GetNode()->GetId() << " is : " << remainingE << std::endl;
		/*

		//Config::SetDefault("ns3::dot11s::HwmpProtocol::SendProactivePReq",BooleanValue(true)); __LINE__

		//nodes.Get(0)->GetDevice(0)->GetObject<MeshWifiInterfaceMac>()->SendProactivePathRequest();
		if(ni[es->GetNode()->GetId()].remainingEAtTreeChangeTime-ni[es->GetNode()->GetId()].remainingE>CHANGE_IN_ENERGY_THRESHOLD)
		{
			//nodes.Get(0)->GetObject<MeshWifiInterfaceMac>()->SendProactivePathRequest();
			Config::Set("/NodeList/ * /DeviceList/ * /$ns3::dot11s::HwmpProtocol/SendProactivePReq",BooleanValue(true));
			// commented by hadi for res std::cout << " setting SendProactivePReq to true now: " << Simulator::Now() << " nod: " << es->GetNode()->GetId() << " remaining energy: " << ni[es->GetNode()->GetId()].remainingE << " " << __FILE__<< ":" << __LINE__ << std::endl;
		}*/
	}

	nextread = Simulator::Now();
	Simulator::Schedule(Seconds(20.0), &MeshTest::ReadRemainingEnergy,this);
}

void
MeshTest::Configure (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.AddValue ("x-size", "Number of nodes in a row grid. [6]", m_xSize);
  cmd.AddValue ("y-size", "Number of rows in a grid. [6]", m_ySize);
  cmd.AddValue ("step",   "Size of edge in our grid, meters. [100 m]", m_step);
  /*
   * As soon as starting node means that it sends a beacon,
   * simultaneous start is not good.
   */
  cmd.AddValue ("start",  "Maximum random start delay, seconds. [0.1 s]", m_randomStart);
  cmd.AddValue ("time",  "Simulation time, seconds [100 s]", m_totalTime);
  cmd.AddValue ("packet-interval",  "Interval between packets in UDP ping, seconds [0.001 s]", m_packetInterval);
  cmd.AddValue ("packet-size",  "Size of packets in UDP ping", m_packetSize);
  cmd.AddValue ("interfaces", "Number of radio interfaces used by each mesh point. [1]", m_nIfaces);
  cmd.AddValue ("channels",   "Use different frequency channels for different interfaces. [0]", m_chan);
  cmd.AddValue ("pcap",   "Enable PCAP traces on interfaces. [0]", m_pcap);
  cmd.AddValue ("stack",  "Type of protocol stack. ns3::Dot11sStack by default", m_stack);
  cmd.AddValue ("root", "Mac address of root mesh point in HWMP", m_root);
  cmd.AddValue ("RANDOM_SEED", "Seed", m_RANDOM_SEED);
  cmd.AddValue ("level", "level", m_level);

  cmd.Parse (argc, argv);
  NS_LOG_DEBUG ("Grid:" << m_xSize << "*" << m_ySize);
  NS_LOG_DEBUG ("Simulation time: " << m_totalTime << " s");
}
void
MeshTest::CreateNodes ()
{ 
  /*
   * Create m_ySize*m_xSize stations to form a grid topology
   */
  nodes.Create (m_ySize*m_xSize);
  // Configure YansWifiChannel
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  /*
   * Create mesh helper and set stack installer to it
   * Stack installer creates all needed protocols and install them to
   * mesh point device
   */
  mesh = MeshHelper::Default ();
  if (!Mac48Address (m_root.c_str ()).IsBroadcast ())
    {
      mesh.SetStackInstaller (m_stack, "Root", Mac48AddressValue (Mac48Address (m_root.c_str ())));
    }
  else
    {
      //If root is not set, we do not use "Root" attribute, because it
      //is specified only for 11s
      mesh.SetStackInstaller (m_stack);
    }
  if (m_chan)
    {
      mesh.SetSpreadInterfaceChannels (MeshHelper::SPREAD_CHANNELS);
    }
  else
    {
      mesh.SetSpreadInterfaceChannels (MeshHelper::ZERO_CHANNEL);
    }
  mesh.SetMacType ("RandomStart", TimeValue (Seconds (m_randomStart)));
  // Set number of interfaces - default is single-interface mesh point
  mesh.SetNumberOfInterfaces (m_nIfaces);
  // Install protocols and return container if MeshPointDevices
  meshDevices = mesh.Install (wifiPhy, nodes);
  // Setup mobility - static grid topology
  MobilityHelper mobility;
//  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
//                                 "MinX", DoubleValue (0.0),
//                                 "MinY", DoubleValue (0.0),
//                                 "DeltaX", DoubleValue (m_step),
//                                 "DeltaY", DoubleValue (m_step),
//                                 "GridWidth", UintegerValue (m_xSize),
//                                 "LayoutType", StringValue ("RowFirst"));
	ns3::RngSeedManager::SetSeed(m_RANDOM_SEED);
	ns3::RngSeedManager::SetRun(m_RANDOM_SEED);

	  Ptr<ListPositionAllocator> initialAlloc = CreateObject<ListPositionAllocator> ();

	  int array_size = 1024;

	  	double * buff1 = new double[500];
	  	double * buff = new double[500];
	  	std::string * array = new std::string[array_size];

	  	std::string * array1 = new std::string[array_size];
	  	int position = 0;

	  	std::ostringstream os;
	  	os << "/home/mesh/app/ns-allinone-3.19/ns-3.19/mojio_report/Topology/locX" << m_RANDOM_SEED << ".txt";
	  	std::ifstream fin(os.str().c_str());
	  	if(fin.is_open())
	  	{
	  		while(!fin.eof())
	  		{
	  			std::getline(fin,array[position]);
	  			position++;
	  		}
	  		array[position-1] = "51";
	  		for(int i = 0; array[i] != "51"; i++)
	  		{
	  			buff[i] = std::atof(array[i].c_str());
	  			std::cout << "FX " << buff[i] << std::endl;
	  		}
	  	}
	  	else
	  	{
	  		std::cout << "File could not be opened." << std::endl;
	  	}
	  	position=0;
	  	std::ostringstream os1;
	  	os1 << "/home/mesh/app/ns-allinone-3.19/ns-3.19/mojio_report/Topology/locY" << m_RANDOM_SEED << ".txt";
	  	std::ifstream fin1(os1.str().c_str());
	  	if(fin1.is_open())
	  	{
	  		while(!fin1.eof())
	  		{
	  			std::getline(fin1,array1[position]);
	  			position++;
	  		}
	  		array1[position-1] = "51";
	  		for(int i = 0; array1[i] != "51"; i++)
	  		{
	  			buff1[i] = std::atof(array1[i].c_str());
	  			std::cout << "FY " << buff1[i] << std::endl;

	  		}
	  	}
	  	else
	  	{
	  		std::cout << "File could not be opened." << std::endl;
	  	}


	  	for (uint32_t i = 0; i < nodes.GetN (); ++i) {
	  		initialAlloc->Add(Vector(buff[i],buff1[i] , 0.0));
	  	}
	  	mobility.SetPositionAllocator(initialAlloc);

	  	//	mobility.SetPositionAllocator("ns3::RandomRectanglePositionAllocator","X",StringValue("ns3::UniformRandomVariable[Min=0.0|Max=300.0]"),"Y",StringValue("ns3::UniformRandomVariable[Min=0.0|Max=300.0]"));
	  	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	  	mobility.Install(nodes);

	// * energy source
	Ptr<Node> node;
	BasicEnergySourceHelper basicSourceHelper;
	EnergySourceContainer sources;
	// * device energy model
	WifiRadioEnergyModelHelper radioEnergyHelper;
	EnergySourceContainer::Iterator esi;
	Ptr<WifiNetDevice> wnd;
	for (NodeContainer::Iterator i = nodes.Begin(); i < nodes.End(); i++) {
		node = *i;

		wnd = node->GetDevice(0)->GetObject<MeshPointDevice>()->GetInterface(1)->GetObject<WifiNetDevice>();

		// commented by hadi for res std::cout << "installing energy model on node:" << node->GetId() << std::endl;
		// configure energy source
		basicSourceHelper.Set("BasicEnergySourceInitialEnergyJ",
				DoubleValue(1000.0));
		// install source
		sources = basicSourceHelper.Install(node);
		// install device model
		esi = sources.Begin();
		es = (*esi)->GetObject<BasicEnergySource>();
		radioEnergyHelper.Install(wnd, es);
		m_esList.push_back(es);
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		//wnd->GetPhy()->GetObject<YansWifiPhy>()->SetEnergySource(es);
		/////////////////////////////////////////////
	}



	Simulator::Schedule(Seconds(0), &MeshTest::ReadRemainingEnergy,this);


  if (m_pcap)
    wifiPhy.EnablePcapAll (std::string ("mp-"));
}
void
MeshTest::InstallInternetStack ()
{
  InternetStackHelper internetStack;
  internetStack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  interfaces = address.Assign (meshDevices);
}
void
MeshTest::InstallApplication ()
{
	    //LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
	    //LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
	UniformVariable rng = UniformVariable();
	double du_time = 50;
	double st_time = 21;
	uint8_t node_src_num;
	int connection_Number=1500;
	double connections_id=1;
	for (int i = 0; connection_Number > 0 ; ++i) {

		st_time = 20;//rng.GetValue(20.0,m_totalTime);  //((double) rand() / (RAND_MAX)) * m_totalTime+10; // 10-simulationDuration
		du_time = 350;//rng.GetValue(100,200) ;   // 0-20

		if (st_time  + du_time > m_totalTime - 200){
			continue;
		}
		else
		{
			connection_Number--;
		}
		node_src_num= rng.GetInteger(1,m_xSize*m_ySize-1);
//		while(node_src_num==1 || node_src_num==4 || node_src_num==2 || node_src_num==5 || node_src_num==8){
//			node_src_num= rng.GetInteger(1,m_xSize*m_ySize-1);
//		}




		bool isInFile=true;
		int array_size = 1024;

		double buff=0;
		std::string * array = new std::string[array_size];
		int position = 0;
		std::ostringstream os;
		os << "/home/mesh/app/Org-allinone-3.19/ns-3.19/mojio_report/l" << m_level << "/Run" << m_RANDOM_SEED << ".txt";
		//os << "/home/mesh/app/Org-allinone-3.19/ns-3.19/mojio_report/l0/Runa1.txt";
                //os << "/home/mesh/app/DC-ns-allinone-3.19/ns-3.19/hadi_report/Run1.txt";
		std::ifstream fin(os.str().c_str());
		if(fin.is_open())
		{
//			std::cout << "File Opened successEL_INFO";
			while(!fin.eof() && position < array_size)
			{
				std::getline(fin,array[position]);
//				std::cout << array[position] << "  AAAAAAAA" << std::endl ;
				position++;
			}
			array[position-1] = "51";
			for(int i = 0; array[i] != "51"; i++)
			{
				buff = std::atof(array[i].c_str());
				if (buff == connections_id){

					isInFile = false;
				}

			}
			if (isInFile){
				std::cout << " REJECTED " << std::endl;
				connections_id++;
				continue;
			}

			std::cout << " ACCEPT " << std::endl;
		}
		else
		{
			std::cout << "File could not be opened." << std::endl;
		}






		//			while(node_src_num==1 || node_src_num==4 || node_src_num==2 || node_src_num==5 || node_src_num==8){
		//				node_src_num= rng.GetInteger(0,m_xSize*m_ySize-1);
		//			}

		std::cout<<" RANDconnection " << (int)connections_id <<" START "<< st_time <<" DURATION "<< du_time <<" FROM "<<(int) node_src_num <<" TO  0 "<<std::endl;

		UdpServerHelper server(connections_id+10000);
		ApplicationContainer serverApps = server.Install(nodes.Get(0));
		serverApps.Start(Simulator::Now());
		serverApps.Stop(Seconds(m_totalTime));


		UdpClientHelper echoClient(interfaces.GetAddress(0), connections_id+10000);
		echoClient.SetAttribute("MaxPackets",UintegerValue((uint32_t) (m_totalTime * (1 / m_packetInterval))));
		echoClient.SetAttribute("Interval", TimeValue(Seconds(m_packetInterval)));
		echoClient.SetAttribute("PacketSize", UintegerValue(m_packetSize));

		ApplicationContainer clientApps = echoClient.Install(nodes.Get(node_src_num));
		clientApps.Start(Seconds(st_time));
		clientApps.Stop(Seconds(st_time+du_time));

		connections_id++;

	}
	std::ostringstream os;
//		os << "/home/mesh/app/Org-allinone-3.19/ns-3.19/mojio_report/level"<< m_level <<"/Run"<< m_RANDOM_SEED <<"/animation.xml";
//		AnimationInterface anim (os.str());
}
int
MeshTest::Run ()
{
  UniformVariable rng = UniformVariable();
  CreateNodes ();
  InstallInternetStack ();
  InstallApplication ();
  Simulator::Schedule (Seconds (m_totalTime), &MeshTest::Report, this);
  Simulator::Stop (Seconds (m_totalTime));

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();
  Simulator::Run ();


  	monitor->CheckForLostPackets ();
  	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier());
  	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  	uint64_t bytesTotal = 0;
  	double lastRxTime=-1;
  	double firstRxTime=-1;
  	for (std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end(); ++i)
  	{
  		if (firstRxTime < 0)
  			firstRxTime = i->second.timeFirstRxPacket.GetSeconds();
  		else if (firstRxTime > i->second.timeFirstRxPacket.GetSeconds() )
  			firstRxTime = i->second.timeFirstRxPacket.GetSeconds();
  		if (lastRxTime < i->second.timeLastRxPacket.GetSeconds() )
  			lastRxTime = i->second.timeLastRxPacket.GetSeconds();
  		bytesTotal = bytesTotal + i->second.rxBytes;
  	}

  	std::cout << "Traffic volume = " << bytesTotal << " bytes" << std::endl;
  	std::cout << "Avg throughput = " << bytesTotal*8/(lastRxTime-firstRxTime)/1024 << " kbits/sec" << std::endl;



  	double delaysum=0;
  	double rxPkt=0;
  	double sum=0;
  	double num=0;
  	for (std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end(); ++i)
  	{
  		delaysum = i->second.delaySum.GetSeconds();
  		rxPkt = i->second.rxPackets;
  		if(rxPkt != 0){
			sum += delaysum / rxPkt;
			std::cout << "delayPerFlow = " << delaysum / rxPkt << " kbits/sec" << std::endl;
  		}
  		num++;

  	}
  	std::cout << "Mean delay = " << sum/num << " sec" << std::endl;

  	double txPkt=0;
  	rxPkt=0;
  	double sumr = 0;
  	double sumt = 0;
  	for (std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end(); ++i)
  	{
  		txPkt = i->second.txPackets;
  		rxPkt = i->second.rxPackets;
  		sumt += txPkt;
  		sumr += rxPkt;
  	}

  	std::cout << "PDR = " << sumr/sumt << " persent" << std::endl;


  	delaysum=0;
  	rxPkt=0;
  	sum=0;
  	num=0;
  	for (std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end(); ++i)
  	{
  		delaysum = i->second.jitterSum.GetSeconds();
  		rxPkt = i->second.rxPackets;
  		sum += delaysum / (rxPkt-1);
  		//std::cout << "delayPerFlow = " << delaysum / rxPkt << " kbits/sec" << std::endl;
  		num++;
  	}
  	std::cout << "Mean jitter = " << sum/num << " sec" << std::endl;

  	sum=0;
  	num=0;
  	double delaysum1=0;
  	sumr = 0;
  	rxPkt = 0;
  	txPkt = 0;
  	delaysum = 0;
  	for (std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end(); ++i)
  	{
  		delaysum = i->second.txBytes;
  		delaysum1= i->second.rxBytes;

  		rxPkt = i->second.timeLastTxPacket.GetSeconds();
  		txPkt = i->second.timeFirstTxPacket.GetSeconds();

  		sum += delaysum/ (rxPkt - txPkt);
  		sumr += delaysum1/ (rxPkt - txPkt);
  		//std::cout << "delayPerFlow = " << delaysum / rxPkt << " kbits/sec" << std::endl;
  		num++;
  	}
  	std::cout << "mean transmit Byterate = " << (sum/num) << " Bite/s" << std::endl;
  	std::cout << "mean received Byterate = " << (sumr/num) << " Bite/s" << std::endl;



  	sum=0;
  	num=0;
  	rxPkt = 0;
  	txPkt = 0;
  	delaysum = 0;
  	for (std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end(); ++i)
  	{
  		if( i->second.rxPackets != 0){
  		delaysum = (i->second.timesForwarded / i->second.rxPackets) + 1;
  		sum += delaysum;
  		}
  		num++;
  	}
  	std::cout << "mean hop count = " << sum/num << std::endl;


  	sum=0;
  	num=0;
  	rxPkt = 0;
  	txPkt = 0;
  	delaysum = 0;
  	for (std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end(); ++i)
  	{
  		delaysum = i->second.lostPackets / (i->second.rxPackets +i->second.lostPackets);
  		sum += delaysum;
  		num++;
  	}
  	std::cout << "packet loss ratio = " << sum/num << std::endl;

  	sum=0;
  	num=0;
  	rxPkt = 0;
  	txPkt = 0;
  	delaysum = 0;
  	for (std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end(); ++i)
  	{
  		delaysum = i->second.lostPackets;
  		sum += delaysum;
  		num++;
  	}
  	std::cout << "packet loss = " << sum/num << std::endl;



  	sum=0;
  		num=0;
  		rxPkt = 0;
  		txPkt = 0;
  		delaysum = 0;
  		for (std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end(); ++i)
  		{
  			delaysum = i->second.txBytes;
  			sum += delaysum;
  			num++;
  		}
  		std::cout << "txBytes = " << sum/num << std::endl;



  	  	sum=0;
  	  		num=0;
  	  		rxPkt = 0;
  	  		txPkt = 0;
  	  		delaysum = 0;
  	  		for (std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end(); ++i)
  	  		{
  	  			std::cout << "------------------" << std::endl;
  	  			std::cout << "timeFirstRxPacket " << i->second.timeFirstRxPacket << std::endl;
  	  			std::cout << "timeLastRxPacket " << i->second.timeLastRxPacket << std::endl;
  	  			std::cout << "timeFirstTxPacket " << i->second.timeFirstTxPacket << std::endl;
  	  			std::cout << "timeLastTxPacket " << i->second.timeLastTxPacket << std::endl;
  	  		}

  	Simulator::Destroy ();
  	return 0;
}
void
MeshTest::Report ()
{
  unsigned n (0);
  for (NetDeviceContainer::Iterator i = meshDevices.Begin (); i != meshDevices.End (); ++i, ++n)
    {
      std::ostringstream os;
      os << "/home/mesh/app/Org-allinone-3.19/ns-3.19/mojio_report/level"<< m_level << "/Run"<< m_RANDOM_SEED <<"/mp-report-" << n << ".xml";
      std::cerr << "Printing mesh point device #" << n << " diagnostics to " << os.str () << "\n";
      std::ofstream of;
      of.open (os.str ().c_str ());
      if (!of.is_open ())
        {
          std::cerr << "Error: Can't open file " << os.str () << "\n";
          return;
        }
      mesh.Report (*i, of);
      of.close ();
    }
}
int
main (int argc, char *argv[])
{
  MeshTest t;
  t.Configure (argc, argv);
  return t.Run ();
}
