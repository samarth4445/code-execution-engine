#!/bin/bash

# Set ulimit values based on the username
case "$USERNAME" in
  user1)
    ulimit -u 2048
    ;;
  user2)
    ulimit -u 128
    ;;
  user3)
    ulimit -u 64
    ;;
  user4)
    ulimit -u 128
    ;;
  user5)
    ulimit -u 64
    ;;
  user6)
    ulimit -u 128
    ;;
  user7)
    ulimit -u 64
    ;;
  user8)
    ulimit -u 128
    ;;
  user9)
    ulimit -u 64
    ;;
  user10)
    ulimit -u 128
    ;;
  user11)
    ulimit -u 64
    ;;
  user12)
    ulimit -u 128
    ;;
  user13)
    ulimit -u 64
    ;;
  user14)
    ulimit -u 128
    ;;
  *)
    # Default ulimit values for unknown users
    ulimit -u 32
    ;;
esac

# Execute the provided command
exec "$@"
