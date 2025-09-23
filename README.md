# gibrash

[![Release](https://img.shields.io/github/release/omilevskyi/gibrash.svg)](https://github.com/omilevskyi/gibrash/releases/latest)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://raw.githubusercontent.com/omilevskyi/gibrash/refs/heads/main/LICENSE)
[![Build](https://github.com/omilevskyi/gibrash/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/omilevskyi/gibrash/actions/workflows/build.yml)
[![Cppcheck](https://github.com/omilevskyi/gibrash/actions/workflows/cppcheck.yml/badge.svg?branch=main)](https://github.com/omilevskyi/gibrash/actions/workflows/cppcheck.yml)
[![Powered By: nFPM](https://img.shields.io/badge/Powered%20by-nFPM-green.svg)](https://github.com/goreleaser/nfpm)

A lightweight utility to show the current git branch.

## Verification of the authenticity

```sh
export VERSION=0.0.2
cosign verify-blob \
  --certificate-identity https://github.com/omilevskyi/gibrash/.github/workflows/release.yml@refs/heads/main \
  --certificate-oidc-issuer https://token.actions.githubusercontent.com \
  --certificate https://github.com/omilevskyi/gibrash/releases/download/v${VERSION}/CHECKSUM.sha256.pem \
  --signature https://github.com/omilevskyi/gibrash/releases/download/v${VERSION}/CHECKSUM.sha256.sig \
  https://github.com/omilevskyi/gibrash/releases/download/v${VERSION}/CHECKSUM.sha256
```
