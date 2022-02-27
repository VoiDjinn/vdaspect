# vdaspect

- [vdaspect](#vdaspect)
  - [Introduction](#introduction)
  - [Functionality](#functionality)
  - [API](#api)
    - [Classes](#classes)
    - [Important Properties and Methods](#important-properties-and-methods)
  - [FAQ and Troubleshooting](#faq-and-troubleshooting)
  - [Glosar](#glosar)
  - [Roadmap](#roadmap)
  - [Deprecated Todos](#deprecated-todos)

## Introduction

VDAspect (System) ist ein Modul für die Godot Engine (3.X).
Das System basiert auf dem Kompsitionsmodell mit einer _flachen_ Vererbung/Herachie. Es ist gedacht für das zusammenstellen von Eigenschaften/Mechaniken (Aspekte).
Dadurch sind sie flexsibel kombinierbar und ergeben Entitäten (zB. Items und Charactere).

## Functionality

Der Kern besteht aus dem `VDAspect` und dem `VDAspectComposer`.

Die Aspekte werden über dem `VDAspectComposer` registriert, anhand ihrer Klasse und ihrer Eltern-Klassen aber ausgenommen der `VDAspect` Klasse.
(TODO: registriert ans ende)

Der `VDAspectComposer` bildet dann am Ende mit den Aspekten die fertige Entität.

Aspekte können Unteraspekte (`Subaspects`) haben. Es gibt zwei Anwendungsfälle.
1. Ein Aspekt erfüllt (Voreingestellt) mehrere Aspekte ( TODO ).
2. Ein Aspekt grupiert (Strucktur Template) mehrere Aspekte.
Auch Unteraspekte werden registriert.

Werend der Registrierung können ungereimtheiten (Kollisionen von Aspekten) auftreten.
Im falle von ungereimtheiten wird eine Fehlermeldung ausgegeben.
Der entsprechende Aspekt wird angetzeigt in der GUI und aktiviert aber der andere Aspekt der Kollision wird deaktivirt.
Die GUI muss also alle Aspekte fest halten.

Aspekte können deaktivirt werden. Es können belibig viele Aspekte von einem Typ gesetztwerden aber nur einer kann aktiv sein.

Unteraspekte müssen geupdatet werden - entweder über Verweise oder über Signale. 

## API

### Classes

* `VDAspect`
* `VDAspectComposer`
* `VDAspectRef`
* `EditorPlugin`

### Important Properties and Methods


## FAQ and Troubleshooting

## Glosar

## Roadmap

* [ ] TODOs auflisten
* [ ] Functionality
* [ ] ???

## Deprecated Todos
* Eventuell kann ein Aspekt auch einen Anderen überschreiben welche regeln sollte es dann geben? Oder ehr nicht?
* Kann der Composer herrausfinden ob ein Aspekt setztbar ist?
* ReadOnly Aspekte?
* Update über *Signale*?
