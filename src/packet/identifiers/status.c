#include "packet/identifiers/status.h"

const char *status_to_s(enum UciStatus status)
{
  switch (status)
  {
  case STATUS_OK:
    return ("Operation completed successfully.");
    break;
  case STATUS_REJECTED:
    return ("Operation was rejected.");
    break;
  case STATUS_FAILED:
    return ("Operation failed.");
    break;
  case STATUS_SYNTAX_ERROR:
    return ("Syntax error in the command.");
    break;
  case STATUS_INVALID_PARAM:
    return ("Invalid parameter provided.");
    break;
  case STATUS_INVALID_RANGE:
    return ("Parameter value out of range.");
    break;
  case STATUS_INVALID_MSG_SIZE:
    return ("Invalid message size.");
    break;
  case STATUS_UNKNOWN_GID:
    return ("Unknown Group Identifier.");
    break;
  case STATUS_UNKNOWN_OID:
    return ("Unknown Opcode Identifier.");
    break;
  case STATUS_READ_ONLY:
    return ("Attempt to write to a read-only parameter.");
    break;
  case STATUS_COMMAND_RETRY:
    return ("Command should be retried.");
    break;
  case STATUS_SESSION_NOT_EXIST:
    return ("Session does not exist.");
    break;
  case STATUS_SESSION_DUPLICATE:
    return ("Duplicate session.");
    break;
  case STATUS_SESSION_ACTIVE:
    return ("Session is already active.");
    break;
  case STATUS_MAX_SESSIONS_EXCEEDED:
    return ("Maximum number of sessions exceeded.");
    break;
  case STATUS_SESSION_NOT_CONFIGURED:
    return ("Session is not configured.");
    break;
  case STATUS_ACTIVE_SESSIONS_ONGOING:
    return ("Active sessions are ongoing.");
    break;
  case STATUS_MULTICAST_LIST_FULL:
    return ("Multicast list is full.");
    break;
  case STATUS_ADDRESS_NOT_FOUND:
    return ("Address not found.");
    break;
  case STATUS_ADDRESS_ALREADY_PRESENT:
    return ("Address is already present.");
    break;
  case STATUS_RANGING_TX_FAILED:
    return ("Ranging transmission failed.");
    break;
  case STATUS_RANGING_RX_TIMEOUT:
    return ("Ranging reception timed out.");
    break;
  case STATUS_RANGING_RX_PHY_DEC_FAILED:
    return ("Ranging reception PHY decode failed.");
    break;
  case STATUS_RANGING_RX_PHY_TOA_FAILED:
    return ("Ranging reception PHY TOA failed.");
    break;
  case STATUS_RANGING_RX_PHY_STS_FAILED:
    return ("Ranging reception PHY STS failed.");
    break;
  case STATUS_RANGING_RX_MAC_DEC_FAILED:
    return ("Ranging reception MAC decode failed.");
    break;
  case STATUS_RANGING_RX_MAC_IE_DEC_FAILED:
    return ("Ranging reception MAC IE decode failed.");
    break;
  case STATUS_RANGING_RX_MAC_IE_MISSING:
    return ("Ranging reception MAC IE missing.");
    break;
  case REASON_RESET_POWER_CYCLE:
    return ("Reset due to power cycle.");
    break;
  case REASON_RESET_SOFTWARE:
    return ("Reset due to software issue.");
    break;
  case REASON_RESET_WATCHDOG:
    return ("Reset due to watchdog timer.");
    break;
  case REASON_RESET_BROWNOUT:
    return ("Reset due to brownout condition.");
    break;
  default:
    return ("");
    break;
  }
}