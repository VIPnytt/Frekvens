### Summary
<!-- Concisely describe the purpose of this PR. Focus on what is being added, changed, or fixed. -->

### Changes
<!-- Bullet list of notable code, logic, or structure changes. -->
<!-- Example:
- Moved diagnostics output behind F_VERBOSE flag
- Fixed mDNS reinitialization on Wi-Fi reconnect
- Replaced Serial.print() chains with Serial.printf()
-->

### Motivation
<!-- Explain the reasoning behind this change. What problem does it solve or what improvement does it bring? -->

### Impact
<!-- Describe the effect on runtime behavior, memory usage, peripheral behavior, user configuration, etc. -->
<!-- Example: Eliminates mDNS setup errors and improves compatibility with OTA extension -->

### Testing
<!-- Briefly describe how this was tested. Include target boards (e.g. ESP32, ESP32-S3), peripherals, or test cases. -->
<!-- Example: Verified on ESP32-S3 with OTA enabled and frequent Wi-Fi reconnects -->

### Related
<!-- Link related issues, discussions, or PRs using # syntax. -->
<!-- Example: Closes #42, Related to #35 -->

---

### Review Checklist

- [ ] Feature or fix has been tested on physical ESP32 hardware
- [ ] All configuration options introduced or changed are documented
- [ ] Changes are backward compatible (or breaking changes are documented)
- [ ] Runtime behavior is verified (e.g. memory usage, serial output, task execution)
- [ ] Console output have been reviewed for unexpected warnings or errors
