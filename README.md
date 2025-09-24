# gibrash

[![Release](https://img.shields.io/github/release/omilevskyi/gibrash.svg)](https://github.com/omilevskyi/gibrash/releases/latest)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://raw.githubusercontent.com/omilevskyi/gibrash/refs/heads/main/LICENSE)
[![Build](https://github.com/omilevskyi/gibrash/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/omilevskyi/gibrash/actions/workflows/build.yml)
[![Cppcheck](https://github.com/omilevskyi/gibrash/actions/workflows/cppcheck.yml/badge.svg?branch=main)](https://github.com/omilevskyi/gibrash/actions/workflows/cppcheck.yml)
[![Powered By: nFPM](https://img.shields.io/badge/Powered%20by-nFPM-green.svg)](https://github.com/goreleaser/nfpm)

A tiny utility that retrieves the git branch name or, if it's HEAD, the git commit short hash.
It is destined to work as quickly as possible in PS1 instead of:

```sh
_get_current_git_branch() {
	local s
	s=$(git --no-pager rev-parse --abbrev-ref HEAD 2>/dev/null)
	test "_${s}" = _HEAD && s=$(git --no-pager rev-parse --short HEAD 2>/dev/null)
	test -z "${s}" || echo " ${s} "
}
PS1="${RESET}\u${PS1_HOST}${CYAN}:${RESET}${BOLD}\w${RESET}\$(_get_current_git_branch)${PS1_TAIL_COLOR}>${RESET} "
```

## Verification of the authenticity

```sh
export VERSION=0.0.1
cosign verify-blob \
  --certificate-identity https://github.com/omilevskyi/gibrash/.github/workflows/release.yml@refs/heads/main \
  --certificate-oidc-issuer https://token.actions.githubusercontent.com \
  --certificate https://github.com/omilevskyi/gibrash/releases/download/v${VERSION}/CHECKSUM.sha256.pem \
  --signature https://github.com/omilevskyi/gibrash/releases/download/v${VERSION}/CHECKSUM.sha256.sig \
  https://github.com/omilevskyi/gibrash/releases/download/v${VERSION}/CHECKSUM.sha256
```
