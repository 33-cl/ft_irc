#!/usr/bin/env bash
PORT=4241
PASS=okcool   # ou vide si tu n’utilises pas PASS

{
  # PASS/NICK/USER
  [ -n "$PASS" ] && printf "PASS %s\r\n" "$PASS"
  printf "NICK SlowBot\r\nUSER sbot 0 * :Slow bot\r\n"
  sleep 1

  # JOIN
  printf "JOIN #chan\r\n"
  sleep 1

  # --- 1ᵉ moitié du PRIVMSG (pas de CRLF) ---
  printf "PRIVMSG #chan :Test par"
  echo "$(date +%T) → avant la pause" >&2

  # pause longue pour tes tests
  sleep 10
  echo "$(date +%T) → après la pause" >&2

  # --- 2ᵉ moitié + CRLF final ---
  printf "tiel\r\n"
} | nc localhost "$PORT"

