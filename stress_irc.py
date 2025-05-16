#!/usr/bin/env python3
import asyncio
import argparse
import random
import sys

async def irc_session(id, host, port, password, channel, results):
    nick = f"user{id}"
    try:
        reader, writer = await asyncio.open_connection(host, port)
    except ConnectionRefusedError:
        print(f"[{nick}] connexion refusée")
        results['refused'] += 1
        return

    try:
        writer.write(f"PASS {password}\r\n".encode())
        writer.write(f"NICK {nick}\r\n".encode())
        writer.write(f"USER {nick} 0 * :stress test\r\n".encode())
        await writer.drain()
    except (ConnectionResetError, BrokenPipeError):
        print(f"[{nick}] perdu en enregistrement")
        results['reset'] += 1
        writer.close()
        return

    await asyncio.sleep(0.1 + random.random() * 0.2)

    try:
        writer.write(f"JOIN {channel}\r\n".encode())
        await writer.drain()
    except (ConnectionResetError, BrokenPipeError):
        print(f"[{nick}] perdu au JOIN")
        results['reset'] += 1
        writer.close()
        return

    await asyncio.sleep(0.5 + random.random() * 0.5)

    try:
        writer.write(f"PART {channel} :Bye\r\n".encode())
        await writer.drain()
    except (ConnectionResetError, BrokenPipeError):
        print(f"[{nick}] perdu au PART")
        results['reset'] += 1
        writer.close()
        return

    try:
        writer.write(b"QUIT :stress test done\r\n")
        await writer.drain()
    except (ConnectionResetError, BrokenPipeError):
        print(f"[{nick}] perdu au QUIT")
        results['reset'] += 1
    finally:
        writer.close()
        try:
            await writer.wait_closed()
        except Exception:
            pass

    results['success'] += 1

async def main():
    parser = argparse.ArgumentParser(description="IRC stress-test robuste")
    parser.add_argument("--host",    default="127.0.0.1", help="Adresse du serveur IRC")
    parser.add_argument("--port",    type=int, default=4240, help="Port du serveur")
    parser.add_argument("--clients", type=int, default=50, help="Nombre de connexions simultanées")
    parser.add_argument("--password",default="monpass",     help="Mot de passe PASS à envoyer")
    parser.add_argument("--channel", default="#42test",      help="Canal à rejoindre")
    parser.add_argument("--delay",   type=float, default=0.0, help="Délai (en secondes) entre chaque connexion")
    args = parser.parse_args()

    results = {'success': 0, 'refused': 0, 'reset': 0}
    tasks = []
    for i in range(args.clients):
        # Crée la tâche puis attend un petit délai
        tasks.append(asyncio.create_task(
            irc_session(i+1, args.host, args.port, args.password, args.channel, results)
        ))
        if args.delay > 0:
            await asyncio.sleep(args.delay)

    await asyncio.gather(*tasks)

    total = args.clients
    print(f"\n=== Bilan du stress-test ({total} sessions lancées) ===")
    print(f"✓ réussies : {results['success']}")
    print(f"× connexions refusées : {results['refused']}")
    print(f"× déconnexions forcées : {results['reset']}")

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nInterrompu par l'utilisateur", file=sys.stderr)
        sys.exit(1)
