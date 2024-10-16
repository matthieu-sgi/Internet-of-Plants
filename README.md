# Internet of Plants

The **Internet of Plants** is an innovative project that revolutionizes the way we interact with and monitor plants. By transforming plants into living sensors, we create a bridge between nature and technology. In this project, we utilize the interaction (touch) you have with the plant and its humidity level to generate real-time sonification of the data. This not only enriches our connection with nature but also provides valuable insights into plant health and well-being.

## Table of Contents

- [Introduction](#introduction)
- [Project Overview](#project-overview)
- [Hardware](#hardware)
- [Software](#software)
- [Internet and networking](#internet-and-networking)
- [Roadmap](#roadmap)

## Introduction

The Internet of Plants is a creative exploration into the realm of biotechnology and sensory augmentation. It enables us to perceive and interpret data from the natural world in a novel and engaging way. With this project, we hope to inspire a deeper appreciation for the environment and promote sustainable practices through data-driven insights. This by conducting a musical art experience.

## Project Overview

### How it Works

The core concept of the Internet of Plants is to use plants as living sensors. We collect data from the plants through two main channels:

1. **Interaction (Touch):** When you interact with a plant by touching its leaves or stems, the project senses this interaction and translates it into a unique sound.

2. **Humidity Level:** We monitor the humidity level of the soil. Changes in humidity are reflected through a different set of sounds, providing valuable feedback on the plant's environmental conditions.

### Repository Structure

    Internet of Plants/
    |-- README.md
    |-- software/
    |   |-- [Software files and source code]
    |-- hardware/
    |   |-- [Hardware design files]
    |-- media/
    |   |-- [Image files related to the project]
    |-- documentation/
    |   |-- poster/
    |   |   |-- [Poster files]
    |   |-- BOM/ # Bill of Materials
    |   |   └── pcb_bom_iop_budget.csv # BOM for the PCB and the budget
    |   |-- user_study/
    |   └──   |-- [User study files]
    └── acknowledgements.md



## Hardware

The hardware for the Internet of Plants is based on a custom-designed Printed Circuit Board (PCB). You can find detailed information about the PCB design, components, and assembly in the **PCB** folder of this repository.

## Software

The software component of this project is responsible for collecting data from the plant sensors and converting it into sonified feedback. It involves signal processing, data visualization, and sound synthesis. Detailed instructions on setting up and running the software can be found in the **Software** section of this repository.

## Internet and networking

The PCBs are made out of an ESP32, a MCU that has Wifi capabilities. The ESP32 is connected to a server that runs a Pure Data. The Pure Data patch is responsible for the sonification of the data.
This allows to connect several and many plants to the same server and to have a centralized sonification of the data.
This is done in order to do a musical art experience and to go to Ars Electronica with the project.

## Deployment

Refer to [deployment.md](https://github.com/matthieu-sgi/Internet-of-Plants/blob/main/deployment.md).

## Roadmap

- [x] Test the electronic filter for the touch interaction
- [x] Test the sound amplification circuit
- [x] Build a prototyping PCB
- [x] Build a software embedded synthesizer
- [x] Do sonification with the prototyping PCB
- [x] Build the "Internet"
- [ ] Build the final PCB
- [x] Build the PureData patch on server
- [ ] Check the correlation between humidity and sensor values
- [x] play some sound from the micro sd
- [ ] check on the maths (the EMA is working well, the ema_std not so much)
- [ ] develop web gui for the esp32 (to change the wifi network)
