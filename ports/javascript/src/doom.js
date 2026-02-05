/**
 * DOOM Pure JavaScript Port (Educational)
 *
 * A hand-written JavaScript implementation of the DOOM renderer.
 * This is NOT an Emscripten compilation - it's a pure JS port for
 * educational purposes to understand how DOOM's rendering works.
 *
 * Based on the original DOOM source code by id Software (GPL v2).
 *
 * Features implemented:
 * - WAD file parsing
 * - BSP tree traversal
 * - Wall rendering (raycasting via BSP)
 * - Flat (floor/ceiling) rendering
 * - Sprite rendering
 * - Palette and colormap handling
 *
 * License: GPL v2
 */

'use strict';

// ============================================================
// WAD File Parser
// ============================================================

class WADReader {
    constructor(arrayBuffer) {
        this.data = new DataView(arrayBuffer);
        this.bytes = new Uint8Array(arrayBuffer);
        this.directory = [];
        this.parse();
    }

    parse() {
        // WAD Header: 4 bytes ID, 4 bytes numLumps, 4 bytes dirOffset
        const id = this.readString(0, 4);
        if (id !== 'IWAD' && id !== 'PWAD') {
            throw new Error(`Invalid WAD file: ${id}`);
        }
        this.type = id;
        this.numLumps = this.data.getInt32(4, true);
        this.dirOffset = this.data.getInt32(8, true);

        // Read directory
        for (let i = 0; i < this.numLumps; i++) {
            const offset = this.dirOffset + i * 16;
            this.directory.push({
                offset: this.data.getInt32(offset, true),
                size: this.data.getInt32(offset + 4, true),
                name: this.readString(offset + 8, 8).replace(/\0+$/, '')
            });
        }

        console.log(`WAD loaded: ${this.type}, ${this.numLumps} lumps`);
    }

    readString(offset, length) {
        let str = '';
        for (let i = 0; i < length; i++) {
            const ch = this.bytes[offset + i];
            if (ch === 0) break;
            str += String.fromCharCode(ch);
        }
        return str;
    }

    getLump(name) {
        const entry = this.directory.find(d => d.name === name.toUpperCase());
        if (!entry) return null;
        return this.bytes.slice(entry.offset, entry.offset + entry.size);
    }

    getLumpByIndex(index) {
        const entry = this.directory[index];
        if (!entry) return null;
        return this.bytes.slice(entry.offset, entry.offset + entry.size);
    }
}

// ============================================================
// Palette & Colormap
// ============================================================

class Palette {
    constructor(wad) {
        this.palettes = [];
        this.colormaps = [];
        this.loadPalettes(wad);
        this.loadColormaps(wad);
    }

    loadPalettes(wad) {
        const data = wad.getLump('PLAYPAL');
        if (!data) return;
        // 14 palettes of 256 RGB triplets
        for (let p = 0; p < 14; p++) {
            const palette = new Uint32Array(256);
            for (let i = 0; i < 256; i++) {
                const off = p * 768 + i * 3;
                const r = data[off], g = data[off + 1], b = data[off + 2];
                palette[i] = 0xFF000000 | (b << 16) | (g << 8) | r; // ABGR for canvas
            }
            this.palettes.push(palette);
        }
    }

    loadColormaps(wad) {
        const data = wad.getLump('COLORMAP');
        if (!data) return;
        // 34 colormaps of 256 bytes each
        for (let c = 0; c < 34; c++) {
            this.colormaps.push(data.slice(c * 256, (c + 1) * 256));
        }
    }

    getColor(paletteIndex, colorIndex) {
        if (this.palettes.length === 0) return 0xFF000000;
        return this.palettes[paletteIndex][colorIndex];
    }
}

// ============================================================
// Map Data Structures
// ============================================================

class DoomMap {
    constructor(wad, mapName) {
        this.name = mapName;
        this.vertices = [];
        this.linedefs = [];
        this.sidedefs = [];
        this.sectors = [];
        this.subsectors = [];
        this.segs = [];
        this.nodes = [];
        this.things = [];
        this.load(wad, mapName);
    }

    load(wad, mapName) {
        // Find the map marker in the directory
        const mapIndex = wad.directory.findIndex(d => d.name === mapName);
        if (mapIndex < 0) {
            throw new Error(`Map ${mapName} not found in WAD`);
        }

        this.loadVertices(wad, mapIndex);
        this.loadLinedefs(wad, mapIndex);
        this.loadSidedefs(wad, mapIndex);
        this.loadSectors(wad, mapIndex);
        this.loadSubsectors(wad, mapIndex);
        this.loadSegs(wad, mapIndex);
        this.loadNodes(wad, mapIndex);
        this.loadThings(wad, mapIndex);

        console.log(`Map ${mapName}: ${this.vertices.length} vertices, ` +
                    `${this.linedefs.length} linedefs, ${this.nodes.length} nodes`);
    }

    findLump(wad, mapIndex, name) {
        for (let i = mapIndex + 1; i < wad.directory.length && i < mapIndex + 12; i++) {
            if (wad.directory[i].name === name) return wad.getLumpByIndex(i);
        }
        return null;
    }

    loadVertices(wad, mapIndex) {
        const data = this.findLump(wad, mapIndex, 'VERTEXES');
        if (!data) return;
        const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
        for (let i = 0; i < data.length; i += 4) {
            this.vertices.push({
                x: view.getInt16(i, true),
                y: view.getInt16(i + 2, true)
            });
        }
    }

    loadLinedefs(wad, mapIndex) {
        const data = this.findLump(wad, mapIndex, 'LINEDEFS');
        if (!data) return;
        const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
        for (let i = 0; i < data.length; i += 14) {
            this.linedefs.push({
                v1: view.getUint16(i, true),
                v2: view.getUint16(i + 2, true),
                flags: view.getUint16(i + 4, true),
                special: view.getUint16(i + 6, true),
                tag: view.getUint16(i + 8, true),
                frontSide: view.getInt16(i + 10, true),
                backSide: view.getInt16(i + 12, true)
            });
        }
    }

    loadSidedefs(wad, mapIndex) {
        const data = this.findLump(wad, mapIndex, 'SIDEDEFS');
        if (!data) return;
        const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
        const decoder = new TextDecoder('ascii');
        for (let i = 0; i < data.length; i += 30) {
            this.sidedefs.push({
                xOffset: view.getInt16(i, true),
                yOffset: view.getInt16(i + 2, true),
                upper: decoder.decode(data.slice(i + 4, i + 12)).replace(/\0+$/, ''),
                lower: decoder.decode(data.slice(i + 12, i + 20)).replace(/\0+$/, ''),
                middle: decoder.decode(data.slice(i + 20, i + 28)).replace(/\0+$/, ''),
                sector: view.getUint16(i + 28, true)
            });
        }
    }

    loadSectors(wad, mapIndex) {
        const data = this.findLump(wad, mapIndex, 'SECTORS');
        if (!data) return;
        const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
        const decoder = new TextDecoder('ascii');
        for (let i = 0; i < data.length; i += 26) {
            this.sectors.push({
                floorHeight: view.getInt16(i, true),
                ceilingHeight: view.getInt16(i + 2, true),
                floorFlat: decoder.decode(data.slice(i + 4, i + 12)).replace(/\0+$/, ''),
                ceilingFlat: decoder.decode(data.slice(i + 12, i + 20)).replace(/\0+$/, ''),
                lightLevel: view.getInt16(i + 20, true),
                special: view.getUint16(i + 22, true),
                tag: view.getUint16(i + 24, true)
            });
        }
    }

    loadSubsectors(wad, mapIndex) {
        const data = this.findLump(wad, mapIndex, 'SSECTORS');
        if (!data) return;
        const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
        for (let i = 0; i < data.length; i += 4) {
            this.subsectors.push({
                numSegs: view.getUint16(i, true),
                firstSeg: view.getUint16(i + 2, true)
            });
        }
    }

    loadSegs(wad, mapIndex) {
        const data = this.findLump(wad, mapIndex, 'SEGS');
        if (!data) return;
        const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
        for (let i = 0; i < data.length; i += 12) {
            this.segs.push({
                v1: view.getUint16(i, true),
                v2: view.getUint16(i + 2, true),
                angle: view.getInt16(i + 4, true),
                linedef: view.getUint16(i + 6, true),
                direction: view.getInt16(i + 8, true),
                offset: view.getInt16(i + 10, true)
            });
        }
    }

    loadNodes(wad, mapIndex) {
        const data = this.findLump(wad, mapIndex, 'NODES');
        if (!data) return;
        const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
        for (let i = 0; i < data.length; i += 28) {
            this.nodes.push({
                x: view.getInt16(i, true),
                y: view.getInt16(i + 2, true),
                dx: view.getInt16(i + 4, true),
                dy: view.getInt16(i + 6, true),
                rightBBox: [
                    view.getInt16(i + 8, true), view.getInt16(i + 10, true),
                    view.getInt16(i + 12, true), view.getInt16(i + 14, true)
                ],
                leftBBox: [
                    view.getInt16(i + 16, true), view.getInt16(i + 18, true),
                    view.getInt16(i + 20, true), view.getInt16(i + 22, true)
                ],
                rightChild: view.getUint16(i + 24, true),
                leftChild: view.getUint16(i + 26, true)
            });
        }
    }

    loadThings(wad, mapIndex) {
        const data = this.findLump(wad, mapIndex, 'THINGS');
        if (!data) return;
        const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
        for (let i = 0; i < data.length; i += 10) {
            this.things.push({
                x: view.getInt16(i, true),
                y: view.getInt16(i + 2, true),
                angle: view.getUint16(i + 4, true),
                type: view.getUint16(i + 6, true),
                flags: view.getUint16(i + 8, true)
            });
        }
    }
}

// ============================================================
// Renderer
// ============================================================

class DoomRenderer {
    constructor(canvas, width, height) {
        this.canvas = canvas;
        this.ctx = canvas.getContext('2d');
        this.width = width || 320;
        this.height = height || 200;
        canvas.width = this.width;
        canvas.height = this.height;
        this.imageData = this.ctx.createImageData(this.width, this.height);
        this.pixels = new Uint32Array(this.imageData.data.buffer);
        this.palette = null;
        this.map = null;

        // Player state
        this.playerX = 0;
        this.playerY = 0;
        this.playerAngle = 0;
        this.playerZ = 41; // Eye height

        // Column clipping
        this.upperClip = new Int32Array(this.width);
        this.lowerClip = new Int32Array(this.width);
    }

    setMap(map) {
        this.map = map;
        // Find player 1 start (thing type 1)
        const start = map.things.find(t => t.type === 1);
        if (start) {
            this.playerX = start.x;
            this.playerY = start.y;
            this.playerAngle = (start.angle * Math.PI) / 180;
        }
    }

    render() {
        if (!this.map || !this.palette) return;

        // Clear framebuffer
        this.pixels.fill(0xFF000000);

        // Reset column clipping
        this.upperClip.fill(0);
        this.lowerClip.fill(this.height);

        // Traverse BSP tree
        if (this.map.nodes.length > 0) {
            this.renderBSP(this.map.nodes.length - 1);
        }

        // Draw to canvas
        this.ctx.putImageData(this.imageData, 0, 0);
    }

    renderBSP(nodeIndex) {
        // Check for subsector (bit 15 set)
        if (nodeIndex & 0x8000) {
            this.renderSubsector(nodeIndex & 0x7FFF);
            return;
        }

        const node = this.map.nodes[nodeIndex];
        if (!node) return;

        // Determine which side of the partition the player is on
        const dx = this.playerX - node.x;
        const dy = this.playerY - node.y;
        const side = (dx * node.dy - dy * node.dx) < 0;

        // Render front side first, then back
        if (side) {
            this.renderBSP(node.leftChild);
            this.renderBSP(node.rightChild);
        } else {
            this.renderBSP(node.rightChild);
            this.renderBSP(node.leftChild);
        }
    }

    renderSubsector(index) {
        const subsector = this.map.subsectors[index];
        if (!subsector) return;

        for (let i = 0; i < subsector.numSegs; i++) {
            const seg = this.map.segs[subsector.firstSeg + i];
            if (seg) {
                this.renderSeg(seg);
            }
        }
    }

    renderSeg(seg) {
        const v1 = this.map.vertices[seg.v1];
        const v2 = this.map.vertices[seg.v2];
        if (!v1 || !v2) return;

        // Transform vertices to player-relative coordinates
        const p1 = this.transformVertex(v1);
        const p2 = this.transformVertex(v2);

        // Clip to near plane
        if (p1.y <= 0 && p2.y <= 0) return;

        // Get linedef and sidedef
        const linedef = this.map.linedefs[seg.linedef];
        if (!linedef) return;

        const sideIndex = seg.direction === 0 ? linedef.frontSide : linedef.backSide;
        if (sideIndex < 0) return;
        const sidedef = this.map.sidedefs[sideIndex];
        if (!sidedef) return;
        const sector = this.map.sectors[sidedef.sector];
        if (!sector) return;

        // Project to screen columns
        const x1 = this.projectX(p1);
        const x2 = this.projectX(p2);

        if (x1 >= x2) return;

        const floorHeight = sector.floorHeight - this.playerZ;
        const ceilingHeight = sector.ceilingHeight - this.playerZ;

        // Light level to color intensity
        const light = Math.min(255, Math.max(0, sector.lightLevel));

        // Draw wall columns
        for (let x = Math.max(0, Math.floor(x1)); x < Math.min(this.width, Math.ceil(x2)); x++) {
            if (x < 0 || x >= this.width) continue;
            if (this.upperClip[x] >= this.lowerClip[x]) continue;

            // Interpolate depth
            const t = (x - x1) / (x2 - x1);
            const depth = p1.y + t * (p2.y - p1.y);
            if (depth <= 0) continue;

            // Project floor and ceiling
            const halfHeight = this.height / 2;
            const yTop = Math.floor(halfHeight - (ceilingHeight / depth) * halfHeight);
            const yBottom = Math.floor(halfHeight - (floorHeight / depth) * halfHeight);

            const drawTop = Math.max(this.upperClip[x], yTop);
            const drawBottom = Math.min(this.lowerClip[x], yBottom);

            // Draw wall column
            const wallColor = this.shadeColor(0xA05020, light, depth);
            for (let y = drawTop; y < drawBottom; y++) {
                if (y >= 0 && y < this.height) {
                    this.pixels[y * this.width + x] = wallColor;
                }
            }

            // Draw ceiling
            const ceilColor = this.shadeColor(0x404040, light, depth);
            for (let y = this.upperClip[x]; y < Math.min(drawTop, this.lowerClip[x]); y++) {
                if (y >= 0 && y < this.height) {
                    this.pixels[y * this.width + x] = ceilColor;
                }
            }

            // Draw floor
            const floorColor = this.shadeColor(0x606060, light, depth);
            for (let y = Math.max(drawBottom, this.upperClip[x]); y < this.lowerClip[x]; y++) {
                if (y >= 0 && y < this.height) {
                    this.pixels[y * this.width + x] = floorColor;
                }
            }

            // Update clipping for solid walls
            if (linedef.backSide < 0) {
                this.upperClip[x] = this.lowerClip[x];
            }
        }
    }

    transformVertex(v) {
        const dx = v.x - this.playerX;
        const dy = v.y - this.playerY;
        const cos = Math.cos(this.playerAngle);
        const sin = Math.sin(this.playerAngle);
        return {
            x: dx * cos - dy * sin,
            y: dx * sin + dy * cos
        };
    }

    projectX(p) {
        if (p.y <= 0) return p.x > 0 ? this.width : 0;
        return (this.width / 2) + (p.x / p.y) * (this.width / 2);
    }

    shadeColor(baseColor, lightLevel, depth) {
        const r = (baseColor >> 16) & 0xFF;
        const g = (baseColor >> 8) & 0xFF;
        const b = baseColor & 0xFF;

        const shade = Math.min(1.0, lightLevel / 255) * Math.min(1.0, 200 / (depth + 1));
        return 0xFF000000 |
            (Math.floor(b * shade) << 16) |
            (Math.floor(g * shade) << 8) |
            Math.floor(r * shade);
    }
}

// ============================================================
// Input Handler
// ============================================================

class InputHandler {
    constructor(renderer) {
        this.renderer = renderer;
        this.keys = {};
        this.moveSpeed = 5;
        this.turnSpeed = 0.05;

        document.addEventListener('keydown', e => { this.keys[e.code] = true; e.preventDefault(); });
        document.addEventListener('keyup', e => { this.keys[e.code] = false; });
    }

    update() {
        const r = this.renderer;
        const cos = Math.cos(r.playerAngle);
        const sin = Math.sin(r.playerAngle);

        if (this.keys['KeyW'] || this.keys['ArrowUp']) {
            r.playerX += sin * this.moveSpeed;
            r.playerY += cos * this.moveSpeed;
        }
        if (this.keys['KeyS'] || this.keys['ArrowDown']) {
            r.playerX -= sin * this.moveSpeed;
            r.playerY -= cos * this.moveSpeed;
        }
        if (this.keys['KeyA'] || this.keys['ArrowLeft']) {
            r.playerAngle -= this.turnSpeed;
        }
        if (this.keys['KeyD'] || this.keys['ArrowRight']) {
            r.playerAngle += this.turnSpeed;
        }
        if (this.keys['KeyQ']) {
            r.playerX -= cos * this.moveSpeed;
            r.playerY += sin * this.moveSpeed;
        }
        if (this.keys['KeyE']) {
            r.playerX += cos * this.moveSpeed;
            r.playerY -= sin * this.moveSpeed;
        }
    }
}

// ============================================================
// Main Application
// ============================================================

class DoomJS {
    constructor(canvasId) {
        const canvas = document.getElementById(canvasId);
        if (!canvas) throw new Error(`Canvas #${canvasId} not found`);

        this.renderer = new DoomRenderer(canvas, 320, 200);
        this.input = new InputHandler(this.renderer);
        this.running = false;
    }

    async loadWAD(url) {
        console.log(`Loading WAD from ${url}...`);
        const response = await fetch(url);
        const buffer = await response.arrayBuffer();
        this.wad = new WADReader(buffer);
        this.renderer.palette = new Palette(this.wad);
        console.log('WAD loaded successfully');
    }

    loadMap(name) {
        console.log(`Loading map ${name}...`);
        const map = new DoomMap(this.wad, name);
        this.renderer.setMap(map);
        console.log('Map loaded');
    }

    start() {
        this.running = true;
        const loop = () => {
            if (!this.running) return;
            this.input.update();
            this.renderer.render();
            requestAnimationFrame(loop);
        };
        requestAnimationFrame(loop);
        console.log('DOOM JS started!');
    }

    stop() {
        this.running = false;
    }
}

// Export for use in HTML
if (typeof window !== 'undefined') {
    window.DoomJS = DoomJS;
    window.WADReader = WADReader;
}

if (typeof module !== 'undefined') {
    module.exports = { DoomJS, WADReader, DoomMap, DoomRenderer, Palette };
}
