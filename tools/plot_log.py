#!/usr/bin/env python3
"""Plot and summarise a sensor log written by serial_log.py.

    python3 tools/plot_log.py data/sensor-logs/<file>.csv

Writes next to the CSV:
    <file>.png   distance over time, one colour band per phase
    <file>.md    table per phase: readings, median, spread, outliers, approach speed

Needs matplotlib once:  pip3 install matplotlib
"""

import csv
import os
import statistics
import sys

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

OUTLIER_CM = 30      # a reading this far from its neighbours' median counts as an outlier
SPEED_FROM_CM = 140  # an approach is timed from the first filtered reading below this ...
SPEED_TO_CM = 40     # ... to the first filtered reading below this


def median5(values):
    """Median of each reading and its 2 neighbours on each side (the filter planned for the Uno)."""
    out = []
    for i in range(len(values)):
        win = values[max(0, i - 2): i + 3]
        out.append(statistics.median(win))
    return out


def load(path):
    rows = []
    with open(path) as f:
        for r in csv.DictReader(f):
            rows.append((float(r["t_s"]), r["phase"], float(r["value"])))
    return rows


def phases(rows):
    """Consecutive runs of the same label -> list of (label, rows)."""
    out = []
    for r in rows:
        if out and out[-1][0] == r[1]:
            out[-1][1].append(r)
        else:
            out.append((r[1], [r]))
    return out


def approach_speed(t, filt):
    t_from = next((ti for ti, v in zip(t, filt) if v < SPEED_FROM_CM), None)
    t_to = next((ti for ti, v in zip(t, filt) if v < SPEED_TO_CM), None)
    if t_from is None or t_to is None or t_to <= t_from:
        return None
    return (SPEED_FROM_CM - SPEED_TO_CM) / 100 / (t_to - t_from)  # m/s


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    path = sys.argv[1]
    base = os.path.splitext(path)[0]
    rows = load(path)
    if not rows:
        sys.exit("The log is empty.")

    t_all = [r[0] for r in rows]
    v_all = [r[2] for r in rows]
    valid = [(t, v) for t, v in zip(t_all, v_all) if v >= 0]
    filt_all = median5([v for _, v in valid])

    span = rows[-1][0] - rows[0][0]
    fig, ax = plt.subplots(figsize=(min(24, max(12, span / 12)), 5))
    colours = plt.rcParams["axes.prop_cycle"].by_key()["color"]
    table = ["| Phase | Readings | Median (cm) | Spread p10–p90 (cm) | Outliers | No echo | Approach 140→40 cm |",
             "|---|---|---|---|---|---|---|"]

    n_label = 0
    for i, (label, pr) in enumerate(phases(rows)):
        t0, t1 = pr[0][0], pr[-1][0]
        if label != "-":
            ax.axvspan(t0, t1, color=colours[i % len(colours)], alpha=0.12, lw=0)
            # alternate two heights so labels of short phases do not overlap
            ax.text((t0 + t1) / 2, 1.01 + 0.05 * (n_label % 2), label, transform=ax.get_xaxis_transform(),
                    ha="center", va="bottom", fontsize=9)
            n_label += 1
        good = [(t, v) for t, _, v in pr if v >= 0]
        no_echo = len(pr) - len(good)
        if label == "-" or len(good) < 3:
            continue
        t = [g[0] for g in good]
        v = [g[1] for g in good]
        filt = median5(v)
        outliers = sum(1 for a, b in zip(v, filt) if abs(a - b) > OUTLIER_CM)
        q = statistics.quantiles(v, n=10)
        speed = approach_speed(t, filt)
        table.append(f"| {label} | {len(pr)} | {statistics.median(v):.1f} | {q[0]:.1f}–{q[-1]:.1f} | "
                     f"{outliers} | {no_echo} | {f'{speed:.2f} m/s' if speed else '–'} |")

    ax.plot([t for t, _ in valid], [v for _, v in valid], ".", ms=3, color="0.55", label="raw reading")
    ax.plot([t for t, _ in valid], filt_all, "-", lw=1.4, color="C0", label="median of 5")
    no_echo_t = [t for t, v in zip(t_all, v_all) if v < 0]
    if no_echo_t:
        ax.plot(no_echo_t, [0] * len(no_echo_t), "x", color="C3", label="no echo (-1)")
    ax.set_xlabel("time (s)")
    ax.set_ylabel("distance from sensor (cm)")
    ax.set_ylim(bottom=-5)
    ax.grid(alpha=0.3)
    ax.legend(loc="upper right", fontsize=9)
    ax.set_title(os.path.basename(base), pad=30)
    fig.tight_layout()
    fig.savefig(base + ".png", dpi=150)

    summary = "\n".join(table)
    with open(base + ".md", "w") as f:
        f.write(f"# {os.path.basename(base)}\n\n![plot]({os.path.basename(base)}.png)\n\n{summary}\n")
    print(summary)
    print(f"\nSaved {base}.png and {base}.md")


if __name__ == "__main__":
    main()
