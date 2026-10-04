# Security threat model

This document will evolve as GB-Sec gains security-relevant features.

At the current stage there is no meaningful security boundary: the device does not yet load untrusted software, store valuable secrets, or expose network services. Security features should therefore not be invented prematurely.

## Future assets

Likely assets include:

- firmware integrity
- game authenticity
- cryptographic keys
- saved data
- device configuration
- debug access
- update mechanisms

## Future attacker capabilities to consider

Potential models may include an attacker who can:

- modify files on removable storage
- connect to exposed UART/SPI/I2C/SWD pads
- submit malformed game files
- observe or alter traffic between components
- possess the device physically
- send data over Wi-Fi or Bluetooth

## Rule for future security work

Every security control should state:

1. the asset it protects;
2. the attacker capability it assumes;
3. the attack it is intended to prevent;
4. the remaining limitations.

Using cryptography alone does not make a design secure.
