#! /bin/bash
#
# Create a a env.local.sh copy of this file and fill in variables.
#

USER="<user>"

HOST="<host>"

HOSTDIR="<location>"

PADIR=$(dirname $(cd $(dirname ${0}) && pwd -P))

WORKSPACE=$(dirname "${PADIR}")

CONFIG="${PADIR}/config"

SCENES="${PADIR}/scenes"

CACHE="${PADIR}/.cache"
