"""Small example OSC server

This program listens to several addresses, and prints some information about
received packets.
"""
import argparse
import math

from pythonosc import dispatcher
from pythonosc import osc_server

from obswebsocket import obsws, requests  # noqa: E402

host = "localhost"
port = 4444
password = ""

ws = obsws(host, port, password)
ws.connect()

GrooveCommandMap = \
{"osc": "/groovegrid/red", "json":""}, \
{"osc": "/groovegrid/spectrum", "json":""}, \
{"osc": "/groovegrid/colorpalette", "json":""}, \
{"osc": "/groovegrid/needforspeed", "json":""}, \
{"osc": "/groovegrid/off", "json":""}, \
{"osc": "/groovegrid/matrix", "json":""}

OBSCommandMap = \
{"osc": "/obs/scene/scene1", "json":""}, \
{"osc": "/obs/scene/scene2", "json":""}, \
{"osc": "/obs/scene/scene3", "json":""}, \
{"osc": "/obs/scene/scene4", "json":""}, \
{"osc": "/obs/scene/scene5", "json":""}, 

ScenesNames = []

def sendToGroovegrid(addr, args):
    for entry in GrooveCommandMap:
        if entry["osc"] == addr:
            print(addr+":"+str(args))

def sendToOBS(addr, args):
    print(addr+":"+str(args))
    scenes = ws.call(requests.GetSceneList())
    if addr == "/obs/scene/scene1":
        ws.call(requests.SetCurrentScene(scenes.getScenes()[0]['name']))
    if addr == "/obs/scene/scene2":
        ws.call(requests.SetCurrentScene(scenes.getScenes()[1]['name']))
    if addr == "/obs/scene/scene3":
        ws.call(requests.SetCurrentScene(scenes.getScenes()[2]['name']))
    if addr == "/obs/scene/scene4":
        ws.call(requests.SetCurrentScene(scenes.getScenes()[3]['name']))
    if addr == "/obs/scene/scene5":
        ws.call(requests.SetCurrentScene(scenes.getScenes()[4]['name']))

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", default="0.0.0.0", help="The ip to listen on")
    parser.add_argument("--port", type=int, default=6250, help="The port to listen on")
    args = parser.parse_args()

    dispatcher = dispatcher.Dispatcher()
    for entry in GrooveCommandMap:
        print("listen to:"+entry["osc"])
        dispatcher.map(entry["osc"], sendToGroovegrid)

    for entry in OBSCommandMap:
        print("listen to:"+entry["osc"])
        dispatcher.map(entry["osc"], sendToOBS)

    server = osc_server.ThreadingOSCUDPServer((args.ip, args.port), dispatcher)
    print("Serving on {}".format(server.server_address))
    server.serve_forever() #endless...

    ws.disconnect()