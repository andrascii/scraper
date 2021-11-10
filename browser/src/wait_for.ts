export function waitFor(msecs: number): Promise<void> {
    return new Promise((resolve: () => void) => {
        // console.log(`Wait for ${Math.round(msecs)} msecs`)
        setTimeout(resolve, Math.round(msecs));
    });
}
