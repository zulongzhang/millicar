/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation;
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

#ifndef SRC_MMWAVE_MODEL_MMWAVE_SIDELINK_MAC_H_
#define SRC_MMWAVE_MODEL_MMWAVE_SIDELINK_MAC_H_

#include "mmwave-sidelink-sap.h"
#include "ns3/mmwave-amc.h"
#include "ns3/mmwave-phy-mac-common.h"
#include "ns3/lte-mac-sap.h"

namespace ns3 {

namespace mmwave {

class MmWaveSidelinkMac : public Object
{

friend class MacSidelinkMemberPhySapUser;

public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Delete default constructor to avoid misuse
   */
  MmWaveSidelinkMac (void) = delete;

  /**
   * \brief Class constructor
   * \param pmc pointer to the MmWavePhyMacCommon instance which specifies the
   *        PHY/MAC parameters
   */
  MmWaveSidelinkMac (Ptr<MmWavePhyMacCommon> pmc);

  /**
   * \brief Class destructor
   */
  ~MmWaveSidelinkMac (void);

  /**
   * \brief Destructor implementation
   */
  virtual void DoDispose (void);

  /**
  * \brief trigger the start of a new slot with all the necessary information
  * \param timingInfo the structure containing the timing information
  */
  void DoSlotIndication (SfnSf timingInfo);

  /**
  * \brief Get the PHY SAP user
  * \return a pointer to the SAP user
  */
  MmWaveSidelinkPhySapUser* GetPhySapUser () const;

  /**
  * \brief Set the PHY SAP provider
  * \param sap the PHY SAP provider
  */
  void SetPhySapProvider (MmWaveSidelinkPhySapProvider* sap);

  /**
  * \brief assign a proper value to the RNTI associated to a specific user
  * \param rnti value of the rnti
  */
  void SetRnti (uint16_t rnti);

  /**
  * \brief return the RNTI associated to a specific user
  * \return the RNTI
  */
  uint16_t GetRnti () const;

  /**
  * \brief set the subframe allocation pattern
  * \param pattern the allocation pattern. The number of element must be equal to
  *        number of slots per subframe. Each element represents the RNTI of the
  *        device scheduled in the corresponding slot.
  */
  void SetSfAllocationInfo (std::vector<uint16_t> pattern);

  /**
  * Transmit PDU function
  */
  void DoTransmitPdu (LteMacSapProvider::TransmitPduParameters params);

private:
  // forwarded from PHY SAP
 /**
  * Receive PHY PDU function
  * \param p the packet
  */
  void DoReceivePhyPdu (Ptr<Packet> p);

  MmWaveSidelinkPhySapUser* m_phySapUser; //!< Sidelink PHY SAP user
  MmWaveSidelinkPhySapProvider* m_phySapProvider; //!< Sidelink PHY SAP provider
  Ptr<MmWavePhyMacCommon> m_phyMacConfig; //!< PHY and MAC configuration pointer
  Ptr<MmWaveAmc> m_amc; //!< pointer to AMC instance
  uint8_t m_mcs; //!< the MCS used to transmit the packets
  uint16_t m_rnti; //!< radio network temporary identifier
  std::vector<uint16_t> m_sfAllocInfo; //!< defines the subframe allocation, m_sfAllocInfo[i] = RNTI of the device scheduled for slot i
  std::list< LteMacSapProvider::TransmitPduParameters > m_txBuffer; //!< buffer containing the packets to be sent
};

class MacSidelinkMemberPhySapUser : public MmWaveSidelinkPhySapUser
{

public:
  MacSidelinkMemberPhySapUser (Ptr<MmWaveSidelinkMac> mac);

  void ReceivePhyPdu (Ptr<Packet> p) override;

  void SlotIndication (SfnSf timingInfo) override;

private:
  Ptr<MmWaveSidelinkMac> m_mac;

};

} // mmwave namespace

} // ns3 namespace

#endif /* SRC_MMWAVE_MODEL_MMWAVE_SIDELINK_MAC_H_ */
