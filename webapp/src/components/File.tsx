import { Device } from '../config/devices';

export const fileImport = (callback: (frames: number[][]) => void) => {
    const csvParser = (file: File, callback: (frames: number[][]) => void) => {
        const reader = new FileReader();
        reader.readAsText(file);
        reader.onload = () => {
            const frames: number[][] = [];
            let frame: number[] = [];
            let y = 0;
            reader.result?.toString().split(/\r?\n/).forEach((row) => {
                let x = 0;
                row.split(",").forEach((column) => {
                    if (column.length && x < Device.GRID_COLUMNS) {
                        frame.push(parseInt(column));
                        ++x;
                    }
                });
                if (x > 0) {
                    while (x < Device.GRID_COLUMNS) {
                        frame.push(0);
                        ++x;
                    }
                    ++y;
                    if (y >= Device.GRID_ROWS) {
                        frames.push(frame);
                        frame = [];
                        y = 0;
                    }
                }
            });
            if (y > 0) {
                while (frame.length < Device.GRID_COLUMNS * Device.GRID_ROWS) {
                    frame.push(0);
                }
                frames.push(frame);
            }
            if (frames.length) {
                callback(frames);
            }
        };
    };

    const imageParser = (file: File, callback: (frame: number[][]) => void) => {
        const reader = new FileReader();
        reader.readAsDataURL(file);
        reader.onload = () => {
            const image = new Image(Device.GRID_COLUMNS, Device.GRID_ROWS);
            image.src = reader.result as string;
            image.onload = () => {
                const canvas = new OffscreenCanvas(Device.GRID_COLUMNS, Device.GRID_ROWS);
                const ctx = canvas.getContext('2d');
                ctx?.drawImage(image, 0, 0, Device.GRID_COLUMNS, Device.GRID_ROWS);
                const imgData = ctx?.getImageData(0, 0, Device.GRID_COLUMNS, Device.GRID_ROWS);
                if (imgData) {
                    const frame: number[] = [];
                    for (var i = 0; i < imgData.data.length; i += 4) {
                        frame.push(imgData.data[i] + imgData.data[i + 1] + imgData.data[i + 2]);
                    }
                    const avg = frame.reduce((p, c, i) => {
                        return p + (c - p) / (i + 1);
                    }, 0);
                    callback([frame.map((i) => (i > avg ? Math.pow(2, 8) - 1 : 0))]);
                }
            };
        };
    };

    const upload = document.createElement('input');
    upload.type = 'file';
    upload.accept = 'text/csv,image/*';
    upload.click();
    upload.onchange = (e: Event) => {
        if (!e.target) {
            return;
        }
        const file = (e.target as HTMLInputElement).files?.[0]!;
        file.name.endsWith('.csv') ? csvParser(file, callback) : imageParser(file, callback);
    };
};

export const csvExport = (name: string, frames: number[][]) => {
    const csvGenerator = (pixels: number[]) => {
        let contents = '';
        let column = 1;
        for (const brightness of pixels) {
            contents += brightness;
            if (column >= Device.GRID_COLUMNS) {
                contents += `\n`;
                column = 1;
            } else {
                contents += ',';
                ++column;
            }
        }
        return contents;
    };

    const handleDownload = (name: string, csv: string) => {
        const element = document.createElement('a');
        element.setAttribute('href', `data:text/csv;charset=utf-8,${encodeURIComponent(csv)}`);
        element.setAttribute('download', `${name.toLowerCase()}.csv`);
        element.style.display = 'none';
        document.body.appendChild(element);
        element.click();
        element.remove();
    };

    let csv = '';
    for (const frame of frames) {
        csv += csvGenerator(frame);
    }
    handleDownload(name, csv);
};
