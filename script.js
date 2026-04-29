const state = { scenarios: {}, data: null, busy: false, autoRunning: false };
let bannerTimer = null;

function setBusy(b) {
    state.busy = b;
    ["run-cycle","reset-state","priority-pulse","auto-clear","capacity-test"].forEach(id => {
        const el = document.getElementById(id);
        if (el) el.disabled = b && !state.autoRunning;
    });
}

async function request(path, payload) {
    setBusy(true);
    try {
        const res = await fetch(path, {
            method: payload !== undefined ? "POST" : "GET",
            headers: { "Content-Type": "application/json" },
            body: payload !== undefined ? JSON.stringify(payload) : undefined
        });
        const data = await res.json();
        if (!res.ok) throw new Error(data.error || "Request failed");
        state.scenarios = data.scenarios;
        state.data = data.state;
        render();
        return data;
    } catch(e) {
        console.error(e);
        state.autoRunning = false;
        alert("Server error: " + e.message + "\n\nMake sure api_server.py is running:\n  python api_server.py");
    } finally { setBusy(false); }
}

async function advanceCycle() {
    await request("/api/run-cycle", {});
    // Show step-by-step summary panel after manual advance
    if (state.data && state.data.history && state.data.history.length > 0) {
        showStepSummary(state.data.history[0]);
    }
}

async function runPriorityPulse() {
    if (state.busy) return;
    for (let i = 0; i < 3; i++) await request("/api/run-cycle", {});
    if (state.data && state.data.history && state.data.history.length > 0) {
        showStepSummary(state.data.history[0]);
    }
}

// Auto-clear runs at 2 seconds per cycle so evaluator can see each change clearly
async function autoClear() {
    if (state.autoRunning) {
        state.autoRunning = false;
        document.getElementById("auto-clear").textContent = "Auto-clear (2s/cycle)";
        document.getElementById("auto-clear-btn").textContent = "Auto-clear (2s/cycle)";
        hideStepSummary();
        return;
    }
    state.autoRunning = true;
    document.getElementById("auto-clear").textContent = "Stop auto-clear";
    document.getElementById("auto-clear-btn").textContent = "Stop auto-clear";
    let maxCycles = 60;
    while (state.autoRunning && maxCycles-- > 0) {
        await request("/api/run-cycle", {});
        const total = Object.values(state.data.lanes).reduce((s, l) => s + l.vehicles, 0);
        if (state.data.history && state.data.history.length > 0) {
            showStepSummary(state.data.history[0]);
        }
        if (total === 0) break;
        await new Promise(r => setTimeout(r, 2000));
    }
    state.autoRunning = false;
    document.getElementById("auto-clear").textContent = "Auto-clear (2s/cycle)";
    document.getElementById("auto-clear-btn").textContent = "Auto-clear (2s/cycle)";
}

function showStepSummary(entry) {
    const box = document.getElementById("step-summary");
    if (!box) return;

    const lane = state.data.lanes[entry.lane];
    const remaining = lane ? lane.vehicles : "?";
    const isEmrg = entry.emergency;

    box.style.display = "block";
    box.className = isEmrg ? "emergency-step" : "";

    document.getElementById("step-title").textContent =
        "Cycle " + entry.cycle + " result" + (isEmrg ? " — EMERGENCY OVERRIDE" : " — Greedy selection");
    document.getElementById("step-lane").textContent = entry.lane + " Lane";
    document.getElementById("step-reason-type").textContent =
        isEmrg ? "Emergency vehicle detected automatically" : "Highest priority score (greedy)";
    document.getElementById("step-priority").textContent =
        isEmrg ? "999999 (emergency override)" :
        (lane ? ((lane.vehicles * lane.wait) / lane.capacity).toFixed(2) : "--");
    document.getElementById("step-green").textContent = entry.green + " seconds";
    document.getElementById("step-cleared").textContent = entry.cleared + " vehicles";
    document.getElementById("step-remaining").textContent = remaining + " vehicles";
    document.getElementById("step-explanation").textContent = state.data.explanation;
}

function hideStepSummary() {
    const box = document.getElementById("step-summary");
    if (box) box.style.display = "none";
}

function showEmergencyBanner(laneName) {
    const banner = document.getElementById("emergency-banner");
    banner.textContent = "EMERGENCY VEHICLE DETECTED — " + laneName.toUpperCase() +
        " LANE — Greedy algorithm bypassed. Emergency lane gets immediate green signal.";
    banner.style.display = "block";
    clearTimeout(bannerTimer);
    bannerTimer = setTimeout(() => { banner.style.display = "none"; }, 5000);
}

function renderScenarioButtons() {
    const c = document.getElementById("scenario-grid");
    c.innerHTML = "";
    Object.entries(state.scenarios).forEach(([key, val]) => {
        const btn = document.createElement("button");
        btn.className = "scenario-button" + (state.data.scenario === key ? " active" : "");
        btn.innerHTML = "<strong>" + val.label + "</strong><span>" + val.detail + "</span>";
        btn.onclick = () => { hideStepSummary(); request("/api/scenario", { scenario: key }); };
        c.appendChild(btn);
    });
}

function renderLanes() {
    Object.entries(state.data.lanes).forEach(([name, lane]) => {
        document.getElementById("count-" + name).textContent = lane.vehicles;
        document.getElementById("meta-" + name).textContent =
            "Wait " + lane.wait + "s | Priority " +
            (lane.priority >= 999999 ? "EMERGENCY" : lane.priority.toFixed(2)) +
            " | Cap " + lane.capacity;
        document.getElementById("badge-" + name).textContent = lane.status;
        document.getElementById("bar-" + name).style.width =
            Math.min((lane.vehicles / lane.capacity) * 100, 100) + "%";
        const card = document.querySelector(".lane-card[data-lane='" + name + "']");
        card.classList.toggle("selected", state.data.selectedLane === name);
        card.classList.toggle("alert", lane.hasEmergency === true);
        card.classList.toggle("blocked", lane.status === "Constrained");
    });
}

function renderAnalytics() {
    const sc = state.scenarios[state.data.scenario];
    document.getElementById("active-pattern").textContent = sc.label + " — " + sc.detail;
    document.getElementById("selected-lane").textContent = state.data.selectedLane;
    document.getElementById("green-time").textContent = state.data.greenTime + "s";
    document.getElementById("signal-caption").textContent = "Green corridor assigned to " + state.data.selectedLane;
    document.getElementById("signal-core-text").textContent = state.data.selectedLane + " corridor has green priority";
    document.getElementById("processed-count").textContent = state.data.processed;
    document.getElementById("average-wait").textContent = state.data.averageWait.toFixed(1) + "s";
    document.getElementById("greedy-estimate").textContent = state.data.greedyEstimate + " vehicles";
    document.getElementById("fixed-estimate").textContent = state.data.fixedEstimate + " vehicles";
    document.getElementById("network-state").textContent = state.data.networkState;
    document.getElementById("network-alert").textContent = state.data.networkAlert;
    document.getElementById("explanation").textContent = state.data.explanation;
    document.getElementById("blockage-toggle").checked = state.data.blockageEast;
    document.getElementById("capacity-test").classList.toggle("active", state.data.blockageEast);

    const cycleEl = document.getElementById("cycle-count");
    if (cycleEl) cycleEl.textContent = state.data.cycle;
    const emrgEl = document.getElementById("emergency-count");
    if (emrgEl) emrgEl.textContent = state.data.emergencyCount;
    const total = Object.values(state.data.lanes).reduce((s, l) => s + l.vehicles, 0);
    const totalEl = document.getElementById("total-vehicles");
    if (totalEl) totalEl.textContent = total + " vehicles";
    const pdEl = document.getElementById("pattern-detail");
    if (pdEl) pdEl.textContent = state.data.scenario.replace(/_/g, " ").toLowerCase();

    if (state.data.lastCycleEmergencyLane) {
        showEmergencyBanner(state.data.lastCycleEmergencyLane);
    }
}

function renderTimeline() {
    const t = document.getElementById("timeline");
    if (!state.data.history || state.data.history.length === 0) {
        t.innerHTML = '<div class="timeline-item"><span class="timeline-step">0</span><div class="timeline-copy"><strong>Ready state</strong><span>Advance a cycle to capture the first allocation record.</span></div><span class="timeline-meta">Standby</span></div>';
        return;
    }
    t.innerHTML = state.data.history.map(e => {
        const emrgClass = e.emergency ? " timeline-emergency" : "";
        const emrgTag = e.emergency ? '<span class="emrg-tag">EMERGENCY OVERRIDE</span>' : "";
        const desc = e.emergency
            ? "Auto-detected emergency. Greedy bypassed. Cleared " + e.cleared + " vehicles in " + e.green + "s."
            : "Greedy selected — highest priority. Cleared " + e.cleared + " vehicles in " + e.green + "s.";
        return '<div class="timeline-item' + emrgClass + '"><span class="timeline-step">' + e.cycle +
            '</span><div class="timeline-copy"><strong>' + e.lane + ' allocated' + emrgTag +
            '</strong><span>' + desc + '</span></div><span class="timeline-meta">Wait ' + e.wait + 's</span></div>';
    }).join("");
}

function renderLaneTable() {
    const c = document.getElementById("lane-table");
    const head = '<div class="lane-table-head"><span>Lane</span><span>Queue</span><span>Average wait</span><span>Priority</span><span>Capacity</span><span>Status</span></div>';
    const rows = Object.entries(state.data.lanes).map(([name, lane]) => {
        const sc = lane.status === "Emergency" ? "alert" : lane.status === "Constrained" ? "blocked" : lane.status === "Allocated" ? "selected" : "";
        const priDisplay = lane.priority >= 999999 ? "999999 (Emergency)" : lane.priority.toFixed(2);
        return '<div class="lane-table-row"><div class="table-main"><strong>' + name + '</strong><span>' +
            (lane.status === "Allocated" ? "Current green allocation" : "Awaiting allocation") +
            '</span></div><div class="table-cell">' + lane.vehicles +
            ' vehicles</div><div class="table-cell">' + lane.wait +
            's</div><div class="table-cell">' + priDisplay +
            '</div><div class="table-cell">' + lane.capacity +
            '</div><div class="table-cell"><span class="table-status ' + sc + '">' + lane.status + '</span></div></div>';
    }).join("");
    c.innerHTML = head + rows;
}

function render() {
    renderScenarioButtons();
    renderLanes();
    renderAnalytics();
    renderTimeline();
    renderLaneTable();
}

document.getElementById("blockage-toggle").addEventListener("change", e => request("/api/toggles", { blockageEast: e.target.checked }));
document.getElementById("run-cycle").addEventListener("click", advanceCycle);
document.getElementById("reset-state").addEventListener("click", () => { hideStepSummary(); request("/api/reset", {}); });
document.getElementById("priority-pulse").addEventListener("click", runPriorityPulse);
document.getElementById("auto-clear").addEventListener("click", autoClear);
document.getElementById("capacity-test").addEventListener("click", () => request("/api/toggles", { blockageEast: !state.data.blockageEast }));

// Panel control buttons — same functions, always visible in left panel
document.getElementById("next-cycle-btn").addEventListener("click", advanceCycle);
document.getElementById("auto-clear-btn").addEventListener("click", autoClear);

request("/api/state");
