# aichmayr-metric-core
Aichmayr vs. Schwarzschield
# φ-Core V1 – Experimental Validation of the Aichmayr Metric

This repository contains the complete Arduino source code and experimental setup used to validate the **Aichmayr metric** through real-time φ(t, r) measurements on ESP32 hardware.

---

## 📖 Project Overview

The **Aichmayr metric** introduces an exponential damping term into the classical Schwarzschild solution:

```
φ(t, r) = 1 - (2GM / r) · e^(–r / r_s)
```

This structure regularizes space-time curvature and removes the singularity at small radii. To verify this behavior, a dual-core setup was constructed using two ESP32 systems, each calculating φ(t, r) in real time and displaying results via OLED.

 Files Included

- `main_esp32_phi.ino`  
  → Full Arduino code for Aichmayr and Schwarzschild metric comparison  
- `aichmayr_phi_bilingual.tex`  
  → Peer-reviewed paper source (LaTeX, English + German)  
- `phi_plot.png`  
  → Comparison plot of φ(t, r) values  
- `/images/`  
  → OLED display photos and test setup

 Hardware Used

- 2× ESP32 (WROOM or DevKit)
- GSR Sensor (analog)
- MPU6050 (I²C)
- QMC5883L Magnetometer
- SH1106 1.3" OLED Display
- SD Card module (SPI)
- Buzzer, LEDs (φ-trigger feedback)

Test Parameters

| Variable | Value |
|----------|-------|
| G        | 1     |
| M        | 1     |
| r        | 10    |
| rₛ       | 5     |

Measured φ(t, r): **0.9729933**  
Expected φ(t, r): **0.972933**

Deviation: ~0.0062%

 Experimental Setup

Each φ-Core runs the same sensor inputs but computes φ(t, r) via a different metric:

- Core A → Schwarzschild (undamped)
- Core B → Aichmayr (damped)

Results are logged to SD card and shown live on OLED.



 📄 Related Publication

> Aichmayr, M. (2025). *GPS Drift & φ(t, r): Evidence for Resonance-Based Time Modulation*. Zenodo.  
> [DOI: 10.5281/zenodo.15640313](https://doi.org/10.5281/zenodo.15640313)



 🔓 License

This project is released under the **MIT License**. Free to use, cite, and build upon.  
Credit: Marcel Aichmayr, 2025 – Independent Research, Austria.
